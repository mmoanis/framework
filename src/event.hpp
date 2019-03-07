// Represents a one pass -execution- of the simulation which implies the
// execution of all the simulation stages.
// A simulation consits of multiple events that are independent of each others.
// Each event is defined by a unique number.
class Event {
public:
    explicit Event(unsigned int number) : number_(number) {
    }

    unsigned int getNumber() const {
        return number_;
    }

private:
    // The number of the event in the simulation
    unsigned int number_ {0};
};
