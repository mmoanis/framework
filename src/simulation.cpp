#include "simulation.hpp"
#include "event.hpp"

#include <array>
#include <iostream>

Simulation::Simulation(const Configuration& config)
        : config_(config), number_of_events_(config_.getNumberOfEvents())
{
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

void Simulation::run()
{
    // simulate the requested number of events
    for (unsigned int i = 0; i < number_of_events_; ++i) {
        Event e(i);

        // generate a random number for each event
        unsigned int seed = random_engine_();

        // execute all modules for this event in specified order
        for (size_t j = 0; j < modules_.size(); ++j) {
            // set the new seed of each module's random engine for this event
            modules_[j]->setSeed(seed);

            // execute module
            modules_[j]->run(&e);
        }
    }
}
