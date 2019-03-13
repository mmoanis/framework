# framework
[![Build Status](https://travis-ci.org/mmoanis/framework.svg?branch=master)](https://travis-ci.org/mmoanis/framework)

Basic and minimalistic framework for running simulations written in C++14. Simulation is defined as modules that are executed in a specific order. Each full execution of the modules is called an event. The total simulation is the result of executing a specified number of events.

# Usage
`framework [-v] configuration_file.conf`

Where the configuration file is a basic config file that shall state the following:
1. `number_of_events` Number of events in simulation.
2. `modules` Modules to include in order. Can be -case sensitive names: Module1, Module2, Module3, Module4, Module5
1. `number_of_threads` Optional number of threads to use. The `-v` option can be used to print execution time.
2. `initial_seed` Optional initial seed for the main random number generator.

# Examples
Sample configuration files and their respective output in the directory `examples`. There are 3 samples and they are as follows:
1. `example.conf`: This is a basic example that loads 5 modules and execute 10 simulation events.
2. `example_initial_seed.conf`: This example shows how to set the initial seed to reproduce the simulation result. By default the seed used to initialize the underlying random number generator is printed at the beginning of the execution for reference.
3. `example_multithread.conf`: This example shows how to use multiple threads to execute the simulation. This option can significantly reduce execution time.

# Building
The project uses `cmake` to generate build files. Please follow the following steps to build the framework using Unix Makefiles.
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

To execute the tests you can use the following command after building the framework:
```
make test
```

# Structure
The repository is structured into 3 directories as follows:
1. `src`: contains all source code of the framework.
2. `examples`: contains a set of configuration files examples.
3. `tests`: contains automated functionality tests and also performance tests and benchmarking scripts.
4. `docs`: contains a complete guide for the framework.
