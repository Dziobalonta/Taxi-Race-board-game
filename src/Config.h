#pragma once

#include <vector>

const int PLAYERS_COUNT = 4;
const int ROAD_SIZE = 191;
const int PASSENGERS_AMOUNT = 44;

struct PassengerCard {
    int cardID;
    int destination; 
    bool inPlay;
};

struct Barrier {
    int fieldIndex;
    int roundsLeft;
};

struct ActiveEvent {
    int eventID;
    int roundsLeft;
};

extern bool useArduino;

// Serial port the Arduino is connected to. On Windows this is "COM3" etc.;
// on macOS find it with: find /dev -maxdepth 1 -name 'cu.usbmodem*'
#ifdef _WIN32
    #define ARDUINO_PORT "COM3"
#else
    #define ARDUINO_PORT "/dev/cu.usbmodem11101"
#endif

extern std::vector<int> mapGraph[ROAD_SIZE];
extern const std::vector<int> DESTINATIONS;
extern const std::vector<int> EVENTS;
extern const std::vector<int> SPAWN_POSITIONS; // valid passenger spawn fields (1-based)

extern int roadState[ROAD_SIZE];
extern char trafficZones[ROAD_SIZE];

extern PassengerCard deck[PASSENGERS_AMOUNT];
extern std::vector<Barrier> activeBarriers;
extern std::vector<ActiveEvent> currentEvents;