#include "simulation.hpp"
#include "event.hpp"
#include "threadPool.hpp"

#include <array>
#include <iostream>
#include <chrono>
using namespace std::chrono_literals;

Simulation::Simulation(const Configuration& config)
        : config_(config), number_of_events_(config_.getNumberOfEvents())
{
    std::cout << "INFO: Using seed= " << config_.getInitialSeed() << std::endl;

    random_engine_.seed(config_.getInitialSeed());
}

// Initialize the simulation modules.
bool Simulation::init()
{
    std::vector<std::string> modules_to_load = config_.getModuleNames();

    // try to create the correct modules
    for (const std::string& module_name : modules_to_load) {
        std::shared_ptr<Module> module = Module::createModule(module_name);
        if (module) {
            std::cout << "INFO: Loaded module: " << module_name << std::endl;
            modules_.push_back(module);
        } else {
            std::cerr << "ERROR: Invalid Modules name: " << module_name << std::endl;
            return false;
        }
    }

    return true;
}

// Run the simulation using the specified number of events.
void Simulation::run()
{
    ThreadPool thread_pool(config_.getNumberOfThreads());
    std::vector<std::future<std::string>> simulation_results(number_of_events_);

    // submit the requested number of events to work queue
    for (unsigned int i = 0; i < number_of_events_; ++i) {
        // generate a random number for each event
        unsigned int seed = random_engine_();
        
        // construct a new event object
        simulation_results[i] = thread_pool.submit([this](std::shared_ptr<Event> e) {
            std::string event_result;

            // simulate the event
            for (auto &module : modules_) {
                event_result += module->run(e);
                //std::this_thread::sleep_for(100ms);
            }

            return event_result;
        }, std::make_shared<Event>(i+1, seed));
    }

    // execute the simulation using specified number of threads
    thread_pool.execute();

    // print the results of the simulation to standard out
    for (unsigned int i = 0; i < number_of_events_; ++i) {
        std::cout << "event #" << i+1 << std::endl;
        std::cout << simulation_results[i].get() << std::endl;
    }
}
