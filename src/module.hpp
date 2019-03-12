#pragma once

#include <random>
#include <string>
#include <memory>
#include <mutex>

class Event;

// Abstract module in the simulation. Module implementations must be derived
// from this class. Every module executes a given event where events can be
// executed in parallel, and as such, each module has it's own mutex which
// should be used when accessing the shared resource or state that is not
// thread safe.
class Module {
public:

	// Virtual destructor as all derived classes are handled with a base pointer.
	virtual ~Module() = default;

	// Factory method for creating modules.
	// params: name - The name of module to create.
	// returns: pointer to the correct class of the module request or null
	// 		if there is no such module.
	static std::shared_ptr<Module> createModule(const std::string& name);

	// Main method for each module. This method is called in each event to
	// execute the module given the information about the current event.
	// params: event - Current event of the simulation.
	// Note: This should be pure virtual.
	virtual std::string run(const Event &);

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
