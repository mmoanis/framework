#include "threadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t number_of_workers)
{
    auto worker = [this]() {
        while (!finished_ || task_queue_.size() > 0) {
            Task task;
            if (popFromTaskQueue(task) && task.valid()) {
                task();
            }
        }
    };

    for (size_t i = 0; i < number_of_workers; ++i) {
        workers_.push_back(std::thread(worker));
    }
}

// Helper method to safely pop an event from the shared task queue.
bool ThreadPool::popFromTaskQueue(Task &t)
{
    std::unique_lock<std::mutex> lock(mutex_);

    // wait until there are events in the task queue to consume
    // or otherwise that we got a signal that there are no more tasks
    // that will be submitted in the future to wait for
    condition_.wait(lock, [this](){
        return task_queue_.size() > 0 || finished_;
    });

    if (task_queue_.size() == 0) {
        return false;
    }

    // get the firt read event
    t = std::move(task_queue_.front());
    task_queue_.pop();

    return true;
}

// Submits an event to be executed by the workers threads.
//
// If there are no workers -ie: the number of workers was set to be zero-
// then the events will be executed in the client's thread.
// Otherwise this will just enqueue the task and let the workers finish
// the job.
ThreadPool::TaskResult ThreadPool::submit(const Event& e)
{
    // create the task function that will be executed
    Task t([e, this](){
        std::lock_guard<std::mutex> lock(mutex_io_);
        std::cout << "Worker #" << std::this_thread::get_id() << " executed event #" << e.getNumber() << '\n';

        return "";
    });

    if (workers_.size() > 0) {
        // add the task to the queue
        submitToTaskQueue(std::move(t));
    } else {
        // execute on caller thread since we have no workers
        t();
    }

    return t.get_future();
}

// Helper method to safely push an event to the shared task queue.
void ThreadPool::submitToTaskQueue(Task && t)
{
    // enter the critical section
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // insert the event in the queue
        task_queue_.push(std::move(t));
    }

    // notify workers waiting for tasks
    condition_.notify_one();
}

// Block waiting for the execution of all submitted events.
// Any tasks submitted after this call will not be executed.
void ThreadPool::execute()
{
    // signal that task submission is done
    finished_.store(true);

    // notify all workers about the changes
    condition_.notify_all();

    // wait for worker threads to finish
    for (auto & worker : workers_) {
        worker.join();
    }
}

