#include "module.hpp"
#include "event.hpp"

// modules are statically linked to the executable for simplicity
#include "module1.hpp"
#include "module2.hpp"
#include "module3.hpp"
#include "module4.hpp"
#include "module5.hpp"

#include <iostream>

// Factory method for creating modules. Each new module should
// declare it self here
// params: name - The name of module to create.
// returns: pointer to the correct class of the module request or null
// 		if there is no such module.
std::shared_ptr<Module> Module::createModule(const std::string& name )
{
    std::shared_ptr<Module> ptr = nullptr;

    if (name == "Module1") {
        ptr = std::make_shared<Module1>();
    } else if (name == "Module2") {
        ptr = std::make_shared<Module2>();
    } else if (name == "Module3") {
        ptr = std::make_shared<Module3>();
    } else if (name == "Module4") {
        ptr = std::make_shared<Module4>();
    } else if (name == "Module5") {
        ptr = std::make_shared<Module5>();
    }

    return ptr;
}

// Main method for each module. This method is called in each event to
// execute the module given the information about the current event.
// Return string of it's name with 2 random numbers drawn from event's
// random number generator.
// params: event - Current event of the simulation.
// Note: This should be pure virtual but since all modules of this example
// do exactly the same thing, for simplicity I keep it.
std::string Module::run(const Event& e, std::mt19937* random_engine)
{
    // draw two random numbers
    unsigned int n1, n2;

    // need to be carefull since the underlying random number engine
    // is not thread safe
    {
        //std::lock_guard<std::mutex> lock(mutex_);
        //random_engine_.seed(e.getSeed());
        n1 = (*random_engine)();
        n2 = (*random_engine)();
    }

    std::string s = name_ + "_" + std::to_string(n1)
                    + "_" + std::to_string(n2) + '\n';
    return s;
}
