#include "event.hpp"

#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

// Execution manager class that allows for parallel execution of events.
// Each event is considered an individal task that are submitted to the
// pool and executed by one of the pool's worker threads. The number of
// worker threads is specified by client of the class.
// Implements a typical producer consumer pattern but with only 1
// producer and one or more consumers.
class ThreadPool
{
public:
    using Task = std::packaged_task<std::string(void)>;
    using TaskResult = std::future<std::string>;

    // Constructor by default assumes the execution is on one thread.
    explicit ThreadPool(size_t number_of_workers = 8);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Submits an event to be executed by the workers threads.
    //
    // If there are no workers -ie: the number of workers was set to be zero-
    // then the events will be executed in the client's thread.
    // Otherwise this will just enqueue the task and let the workers finish
    // the job.
    TaskResult submit(const Event& e);

    // Block waiting for the execution of all submitted events.
    // Any tasks submitted after this call will not be executed.
    void execute();

private:
    // Helper method to safely push an event to the shared task queue.
    void submitToTaskQueue(Task && t);

    // Helper method to safely pop an event from the shared task queue.
    bool popFromTaskQueue(Task &t);

    // conditional variable for managing the shared queue
    std::condition_variable condition_;

    // internal state of the executer. This flag is set when execute method is
    // called to signal the workers to finish the tasks they have.
    std::atomic_bool finished_ {false};

    // task queue
    std::queue<std::packaged_task<std::string(void)>> task_queue_;

    // mutex for making thread safe operations on the task queue.
    std::mutex mutex_;

    // mutex for writing to IO
    std::mutex mutex_io_;

    // worker threads
    std::vector<std::thread> workers_;
};