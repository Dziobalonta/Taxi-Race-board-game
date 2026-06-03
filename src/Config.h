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

extern std::vector<int> mapGraph[ROAD_SIZE];
extern const std::vector<int> DESTINATIONS;
extern const std::vector<int> EVENTS;

extern int roadState[ROAD_SIZE];
extern char trafficZones[ROAD_SIZE];

extern PassengerCard deck[PASSENGERS_AMOUNT];
extern std::vector<Barrier> activeBarriers;
extern std::vector<ActiveEvent> currentEvents;