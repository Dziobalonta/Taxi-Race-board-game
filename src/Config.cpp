#include "Config.h"

bool useArduino = false;

std::vector<int> mapGraph[ROAD_SIZE];

// index - index of a destination point(blue fields), value - index of the field on board
const std::vector<int> DESTINATIONS = {
    40, 44, 14, 4, 32, 48, 140, 70, 148, 23, 58, 81, 168, 110, 89, 125, 157, 163,
};

// Fields where player recieve Event cards are also their random spawn points
const std::vector<int> EVENTS = {
    9, 21, 36, 50, 75, 87, 103, 111, 161, 172,
};

int roadState[ROAD_SIZE];
char trafficZones[ROAD_SIZE];

PassengerCard deck[PASSENGERS_AMOUNT];
std::vector<Barrier> activeBarriers;
std::vector<ActiveEvent> currentEvents;