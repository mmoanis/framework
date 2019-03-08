#include "simulation.hpp"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

int main() {
	unsigned int number_of_events = 0, seed;
	cout << "Framework ..." << endl;
	cout << "Enter the required number of events to simulate: ";
	cin >> number_of_events;
	cout << "Enter the initial seed: ";
	cin >> seed;

	Simulation simulation(number_of_events, seed);
	if (simulation.init()) {
		simulation.run();
	} else {
		std::cerr << "Terminating ..." << endl;
	}

	return 0;
}
