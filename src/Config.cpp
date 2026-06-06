#include "Config.h"

bool useArduino = true;

std::vector<int> mapGraph[ROAD_SIZE];

// index - index of a destination point(blue fields), value - index of the field on board
const std::vector<int> DESTINATIONS = {
    40, 44, 14, 4, 32, 48, 140, 70, 148, 23, 58, 81, 168, 110, 89, 125, 157, 163,
};

// Fields where player recieve Event cards are also their random spawn points
const std::vector<int> EVENTS = {
    9, 21, 36, 50, 75, 87, 103, 111, 161, 172,
};

// Fields where passengers may spawn (1-based). Restricted to real road fields
// that have a mapped LED, so spawns always show up on the board.
const std::vector<int> SPAWN_POSITIONS = {
    1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 18, 22, 23, 26, 28, 29, 30, 31, 32, 35, 36,
    37, 38, 40, 41, 42, 43, 44, 45, 46, 48, 50, 54, 58, 59, 60, 61, 62, 65, 66,
    68, 69, 73, 74, 75, 76, 77, 78, 79, 81, 82, 83, 84, 86, 87, 88, 89, 90, 91,
    92, 94, 95, 100, 101, 102, 104, 105, 107, 108, 109, 110, 112, 113, 114, 115,
    116, 119, 120, 121, 122, 127, 128, 129, 130, 132, 135, 136, 139, 140, 141,
    142, 143, 144, 146, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 161,
    164, 165, 168, 169, 170, 171, 172, 173, 174, 175, 177, 179, 180, 183, 184,
    185, 187, 188, 189, 190, 191
};

int roadState[ROAD_SIZE];
char trafficZones[ROAD_SIZE];

PassengerCard deck[PASSENGERS_AMOUNT];
std::vector<Barrier> activeBarriers;
std::vector<ActiveEvent> currentEvents;