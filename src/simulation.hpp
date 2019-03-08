#pragma once

#include <random>
#include <vector>

#include "module.hpp"

// The main simulation engine in the framework. Controlls the modules
// and the execution of all simulation events.
class Simulation
{
public:
    explicit Simulation(unsigned int number_of_events, unsigned int seed);

    // Initialize the simulation modules.
    bool init();

    // Run the simulation using the specified number of events.
    void run();

private:
    // list of loaded modules in the simulation
    std::vector<std::shared_ptr<Module>> modules_;

    // total number of events in the simulation
    unsigned int number_of_events_ {0};

    // mersenne twister pseudo-random number generator.
	std::mt19937 random_engine_;
};
