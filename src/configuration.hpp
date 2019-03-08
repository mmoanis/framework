#pragma once

#include <vector>
#include <string>
#include <map>

// Simulation configuration file. Very basic configuration file that reads
// basic information about the simulation.
// File contains key value pairs.
class Configuration
{
public:
    // Create the configuration object from the configuration file.
    static Configuration createConfiguration(std::string config_file_path);

    // Return whether the configuration is correct or not.
    bool correct() const { return correct_; }

    // Returns the user specified initial seed.
    unsigned int getInitialSeed() const {
        return initial_seed_;
    }

    // Returns the list of module names in the configuration file
    std::vector<std::string> getModuleNames() const {
        return module_names_;
    }

    // Returns the user specified number of events.
    unsigned int getNumberOfEvents() const {
        return number_of_events_;
    }

    // Returns the user specified number of threads.
    unsigned int getNumberOfThreads() const {
        return number_of_threads_;
    }

private:
    Configuration() = default;

    // whether the configuration is correct or no
    bool correct_ {false};

    // optional number to be used as a seed for random number generator.
    unsigned int initial_seed_;

    // module names sorted as they appeared in the configuration file
    std::vector<std::string> module_names_;

    // total number of events
    unsigned int number_of_events_ {0};

    // optional number specifing the number of threads to use. Default is zero.
    unsigned int number_of_threads_ {0};
};
