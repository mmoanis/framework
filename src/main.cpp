#include "configuration.hpp"
#include "simulation.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;
using std::cout;
using std::endl;
using std::cin;

int main(int argc, char* argv[]) {
	int return_code = -1;
	cout << "Framework ..." << endl;

	if (argc != 2) {
		std::cerr << "ERROR: Incorrect number of arguments\n";
		std::cerr << "Usage: framework /path/to/configuration.file\n";
		return return_code;
	}

	Configuration config = Configuration::createConfiguration(argv[1]);
	if (config.correct()) {
		Simulation simulation(config);
		if (simulation.init()) {
			high_resolution_clock::time_point start_time = high_resolution_clock::now();
			
			// execute the simulation
			simulation.run();
			
			high_resolution_clock::time_point finish_time = high_resolution_clock::now();

			auto duration = duration_cast<microseconds>( finish_time - start_time ).count();
			cout << "INFO: Finished simulation with " 
				 << config.getNumberOfEvents() << " events in " << duration << "us\n";

			// exit normally
			return_code = 0;
		}
	} else {
		std::cerr << "Incorrect configuration file...\n";
	}
	std::cout << "Terminating ..." << endl;

	return return_code;
}
