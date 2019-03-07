#include "module.hpp"
#include "event.hpp"

// Factory method for creating modules. Each new module should
// declare it self here
// params: name - The name of module to create.
// returns: pointer to the correct class of the module request or null
// 		if there is no such module.
std::shared_ptr<Module*> Module::createModule(const std::string& name )
{
    // TODO: inistantiate the correct class for a given module name
    return nullptr;
}

// Main method for each module. This method is called in each event to
// execute the module given the information about the current event.
// Return string of it's name with 2 random numbers drawn from engine.
// params: event - Current event of the simulation.
// Note: This should be pure virtual but since all modules of this example
// do exactly the same thing, for simplicity I keep it.
void Module::run(Event *e)
{
    // TODO: should return the string? print? or save in event!
    std::string s = name_ + std::to_string(random_engine_())
                    + std::to_string(random_engine_());
    (void)s;
}

// Set the seed of the underlying random number generator.
void Module::setSeed(unsigned int seed)
{
    random_engine_.seed(seed);
}