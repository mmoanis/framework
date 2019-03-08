#include "simulation.hpp"
#include "event.hpp"

#include <array>
#include <iostream>

Simulation::Simulation(unsigned int number_of_events, unsigned int seed)
        : number_of_events_(number_of_events)
{
    random_engine_.seed(seed);
}

// Initialize the simulation modules.
bool Simulation::init()
{
    // the modules that need to be loaded in the simulation
    const std::array<std::string, 5> modules_to_load = {
        "Module1",
        "Module2",
        "Module3",
        "Module4",
        "Module5"
    };

    // try to create the correct modules
    for (size_t i = 0; i < modules_to_load.size(); ++i) {
        std::shared_ptr<Module> module = Module::createModule(modules_to_load[i]);
        if (module) {
            std::cout << "INFO: Loaded module: " << modules_to_load[i] << std::endl;
            modules_.push_back(module);
        } else {
            std::cerr << "ERROR: Invalid Modules name: " << modules_to_load[i] << std::endl;
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
