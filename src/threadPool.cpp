#include "threadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t number_of_workers)
{
    auto worker = [this]() {
        while (true) {
            InternalTaskType task;

            // try to get a task to execute from the shared work queue.
            // this section is considered critical as race conditions
            // can happen due to the fact that the underlying queue is
            // not thread safe.
            {
                // lock the critical section since we are going to do
                // some operations on the shared queue
                std::unique_lock<std::mutex> lock(mutex_);

                // wait until there are events in the task queue to consume
                // or otherwise that we got a signal that there are no more tasks
                // that will be submitted in the future to wait for
                condition_.wait(lock, [this](){
                    return !task_queue_.empty() || finished_;
                });

                // we need to check after waking up if the wake up signal means
                // that there are no more work to be done
                if (finished_ && task_queue_.empty()) {
                    break;
                }

                // get the task out of the queue  
                task = std::move(task_queue_.front());
                task_queue_.pop();
            }
            
            // execute the task
            task();
        }
    };

    // construct the worker threads
    for (size_t i = 0; i < number_of_workers; ++i) {
        workers_.push_back(std::thread(std::move(worker)));
    }
}

// Submits an event to be executed by the workers threads.
//
// If there are no workers -ie: the number of workers was set to be zero-
// then the events will be executed in the client's thread.
// Otherwise this will just enqueue the task and let the workers finish
// the job.
ThreadPool::TaskResult ThreadPool::submit(ThreadPool::TaskType&& func,
    ThreadPool::TaskParamType&& param)
{
    TaskResult result;

    // in case there are no worker threads, we are going to directly execute
    // the task on the caller thread since we have none.
    if (workers_.size() > 0) {
        // allocate task to be executed
        auto task = std::make_shared<std::packaged_task<std::string()>> (
            std::bind(std::forward<TaskType>(func), std::forward<TaskParamType>(param)));
        result = task->get_future();

        // insert the task in the work queue
        // this is a critical section since we are modifying a shared resource
        {
            // lock the work queue mutex
            std::lock_guard<std::mutex> lock(mutex_);

            // insert the event in the queue wrapped by a simple function
            // with no return type
            task_queue_.emplace([task](){(*task)();});
        }

        // notify one of the workers waiting for tasks
        condition_.notify_one();
    } else {
        // execute on caller thread since we have no workers
        // no need for any heap allocations
        std::packaged_task<std::string(Event)> task(func);
        result = task.get_future();

        task(param);
    }

    return result;
}

// Block waiting for the execution of all submitted events.
// Any tasks submitted after this call will not be executed.
void ThreadPool::execute()
{
    // signal that task submission is done
    {
        // lock the critical section using the same mutex
        // since we are going to modify a shared variable
        // that other threads are waiting on
        std::unique_lock<std::mutex> lock(mutex_);
        finished_ = true;
    }

    // notify all workers about the changes
    condition_.notify_all();

    // wait for worker threads to finish
    for (auto & worker : workers_) {
        worker.join();
    }
}

