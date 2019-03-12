#include "configuration.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

// Helper to parse a number
unsigned int parseNumber(std::string number)
{
    unsigned int value;
    try {
        value = std::stoul(number, nullptr, 10);
    } catch (...) {
        std::cerr << "ERROR: Invalid numeric value\n";
        throw;
    }

    return value;
}

Configuration Configuration::createConfiguration(std::string config_file_path)
{
    Configuration config;

    // open config file
    std::ifstream config_file(config_file_path);
    if (!config_file) {
        std::cerr << "ERROR: Couldn't open configuration file " << config_file_path << '\n';
        return config;
    }

    // what we are looking for to consider this file as correct and return a valid object
    bool seen_number_of_events_before = false;
    bool seen_modules_before = false;
    bool seen_seed_before = false;

    // read the file line by line expecting a key-value pair on each line.
    std::string line;
    while (getline(config_file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream tokenizer;
        tokenizer << line;

        std::string key, equal_sign, value;
        tokenizer >> key >> equal_sign >> value;
        if (equal_sign != "=" || tokenizer.bad()) {
            std::cerr << "ERROR: Unexpected token " << equal_sign << '\n';
            return config;
        }

        // match the key with any of the keywords
        if (key == "number_of_events") {
            if (seen_number_of_events_before) {
                std::cerr << "ERROR: Number of events was defined multiple times\n";
                return config;
            } else {
                try {
                    config.number_of_events_ = parseNumber(value);
                } catch (...) {
                    return config;
                }
                seen_number_of_events_before = true;
            }
        } else if (key == "number_of_threads") {
            try {
                config.number_of_threads_ = parseNumber(value);
            } catch (...) {
                return config;
            }
        } else if (key == "initial_seed") {
            try {
                config.initial_seed_ = parseNumber(value);
            } catch (...) {
                return config;
            }
            seen_seed_before = true;
        } else if (key == "modules") {
            if (seen_modules_before) {
                std::cerr << "ERROR: Modules were defined multiple times\n";
                return config;
            } else {
                do {
                    config.module_names_.push_back(value);
                } while (tokenizer >> value);
                seen_modules_before = true;
            }
        }
    }

    // initialize the seed if not specified in file
    if (!seen_seed_before) {
        config.initial_seed_ = static_cast<unsigned int>(time(NULL));
    }

    // check we have the needed values
    config.correct_ = seen_number_of_events_before && seen_modules_before && config.module_names_.size() > 0;

    return config;
}
