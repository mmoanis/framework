#include "threadPool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t number_of_workers)
{
    auto worker = [this]() {
        while (!finished_ || task_queue_.size() > 0) {
            Event* e = popFromTaskQueue();
            if (e) {
                //e->run();
                (void)e;

                std::lock_guard<std::mutex> lock(mutex_io_);
                std::cout << "Worker #" << std::this_thread::get_id() << " executed event #" << e->getNumber() << '\n';
            }
        }
    };

    for (size_t i = 0; i < number_of_workers; ++i) {
        workers_.push_back(std::thread(worker));
    }
}

// Helper method to safely pop an event from the shared task queue.
Event* ThreadPool::popFromTaskQueue()
{
    std::unique_lock<std::mutex> lock(mutex_);

    // wait until there are events in the task queue to consume
    // or otherwise that we got a signal that there are no more tasks
    // that will be submitted in the future to wait for
    condition_.wait(lock, [this](){
        return task_queue_.size() > 0 || finished_;
    });

    // get the firt read event
    Event * e = nullptr;
    if (task_queue_.size() > 0) {
        e = task_queue_.front();
        task_queue_.pop();
    } 

    return e;
}

// Submits an event to be executed by the workers threads.
//
// If there are no workers -ie: the number of workers was set to be zero-
// then the events will be executed in the client's thread.
// Otherwise this will just enqueue the task and let the workers finish
// the job.
void ThreadPool::submit(Event *e)
{
    if (workers_.size() > 0) {
        // add the task to the queue
        submitToTaskQueue(e);
    } else {
        // execute on caller thread since we have no workers
        //e->run();
        std::cout << "Executed event #" << e->getNumber() << " on main thread\n";
    }

}

// Helper method to safely push an event to the shared task queue.
void ThreadPool::submitToTaskQueue(Event *e)
{
    // enter the critical section
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // insert the event in the queue
        task_queue_.push(e);
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

