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
        : number_(number) {
        random_engine_.seed(seed);
    }

    // Copys are disallowed
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    // Returns the number of this event.
    unsigned int getNumber() const {
        return number_;
    }

    // Returns a random number to be used specifically in this event.
    unsigned int getRandomNumber() {
        return random_engine_();
    }

private:
    // the id of the event in the simulation.
    unsigned int number_ {0};

    // random number generator associated with this event.
    std::mt19937 random_engine_;
};
