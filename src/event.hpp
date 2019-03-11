#pragma once

#include <random>

// Event is a full execution of a simulation. Each event is identified by an ID.
// Additionally, each event has its own random number generator that can be
// used to draw random numbers specific to this event. This allows for
// simulations to be reproducable.
class Event {
public:
    // Construct an event with a given number and an initial seed to
    // the random numbers generated in this event.
    explicit Event(unsigned int number, unsigned int seed)
        : number_(number), seed_(seed) {}

    // Returns the number of this event.
    unsigned int getNumber() const {
        return number_;
    }

    // Returns the seed to be used by the random number generator
    unsigned int getSeed() const {
        return seed_;
    }

private:
    // the id of the event in the simulation.
    unsigned int number_ {0};

    // seed used for this event
    unsigned int seed_ {0};
};
