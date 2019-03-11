#pragma once

#include <random>
#include <string>
#include <memory>
#include <mutex>

class Event;

// Abstract module in the simulation. Module implementations must be derived
// from this class.
class Module {
public:

	// Factory method for creating modules.
	// params: name - The name of module to create.
	// returns: pointer to the correct class of the module request or null
	// 		if there is no such module.
	static std::shared_ptr<Module> createModule(const std::string& name );

	// Main method for each module. This method is called in each event to
	// execute the module given the information about the current event.
	// params: event - Current event of the simulation.
	// Note: This should be pure virtual.
	virtual std::string run(std::shared_ptr<Event> &e);

	// Set the seed of the underlying random number generator.
	// void setSeed(unsigned int seed);

protected:
	// Constructor of the abstract class. It's made protected to enforce this
	// class being abstract and only derived classes can be instantiated.
	Module(std::string name) : name_(name) {}

	// module unique name
	std::string name_;

	// mersenne twister pseudo-random number generator.
	std::mt19937 random_engine_;

	// associated mutex to access the random number
	std::mutex mutex_;
};
