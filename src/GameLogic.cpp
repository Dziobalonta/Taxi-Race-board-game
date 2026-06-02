#include "GameLogic.h"
#include "Config.h"

#include <iostream>
#include <cstdlib>

using namespace std;

void initGame() {
    for(int i = 0; i < ROAD_SIZE; i++) {
        // 0 - empty road
        // 1 - slight traffic
        // 2 - heavy traffic
        // 3 - Player's spawn point
        // 4 - Passenger's spawn point
        // 5 - Temporary Barrier
        roadState[i] = 0;
    }
}

void setZoneFromList(const vector<int>& fields, char zone) {

    for (size_t i = 0; i < fields.size(); i++) {
        int fieldIndex = fields[i] - 1;

        // cannot be bigger than whole road
        if (fieldIndex >= 0 && fieldIndex < ROAD_SIZE) {
            trafficZones[fieldIndex] = zone;
        }
    }
}

void initZones() {
    // if forgotten - be in zone A
    for (int i = 0; i < ROAD_SIZE; i++) {
        trafficZones[i] = 'A';
    }

    setZoneFromList({ 177, 176, 175, 174, 66, 65, 64, 63, 62, 61, 146, 145, 144, 143, 142, 141, 140, 28, 27, 26, 25, 24 }, 'A'); 
    setZoneFromList({ 23, 22, 21, 20, 19, 18, 17, 16, 15, 67, 68, 139, 138, 137, 191 }, 'B');
    setZoneFromList({ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 'C');
    setZoneFromList({ 136, 135, 134, 133, 132, 131, 130, 69, 70, 71, 72, 73, 74, 75, 76, 1 }, 'D');
    setZoneFromList({ 129, 128, 127, 126, 125, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93 }, 'E');
    setZoneFromList({ 96, 95, 94, 124, 123, 122, 121, 120, 173, 172, 171, 170, 169, 168 }, 'F');
    setZoneFromList({ 167, 166, 165, 164, 163, 162, 161, 97, 98, 99, 100, 119, 118, 117, 116 }, 'G');
    setZoneFromList({ 152, 153, 154, 155, 156, 157, 158, 159, 160, 113, 114, 115, 102, 101 }, 'H');
    setZoneFromList({ 103, 104, 105, 106, 107, 108, 109, 110, 111, 112 }, 'I');
    setZoneFromList({ 54, 55, 56, 57, 58, 59, 60, 147, 148, 149, 150, 151 }, 'J');
    setZoneFromList({ 29, 30, 31, 178, 179, 180, 53, 52, 51, 50, 49, 48, 47, 186, 187, 188, 189, 190 }, 'K');
    setZoneFromList({ 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 181, 182, 183, 184, 185 }, 'L');
}

int getNextTrafficState(int currentState, char zone) {
    // protection from players and passengers
    if (currentState > 2) currentState = 0; 

    int chance = rand() % 100;
    
    switch (zone) {
        case 'A': 
        case 'B':
        case 'G':
        case 'H':
            // High traffic areas
            if (currentState == 0) return (chance < 40) ? 1 : 0;
            if (currentState == 1) return (chance < 40) ? 2 : ((chance < 80) ? 1 : 0);
            if (currentState == 2) return (chance < 80) ? 2 : 1;
            break;

        case 'D': 
        case 'E':
        case 'J':
        case 'L':
            // Medium traffic areas
            if (currentState == 0) return (chance < 20) ? 1 : 0;       
            if (currentState == 1) return (chance < 20) ? 2 : ((chance < 60) ? 1 : 0); 
            if (currentState == 2) return (chance < 60) ? 2 : 1;
            break;

        case 'C':
        case 'F':
        case 'I':
        case 'K':
            // Almost No Traffic areas
            if (currentState == 0) return (chance < 5) ? 1 : 0;
            if (currentState == 1) return (chance < 10) ? 2 : ((chance < 40) ? 1 : 0);
            if (currentState == 2) return (chance < 30) ? 2 : 1;
            break;

        default: // DEFAULT
            if (currentState == 0) return (chance < 25) ? 1 : 0;
            if (currentState == 1) return (chance < 25) ? 2 : ((chance < 60) ? 1 : 0);
            if (currentState == 2) return (chance < 70) ? 2 : 1;
            break;
    }
    
    return 0; // if unresolved return no traffic
}

int getCurrentZoneState(char zone) {
    for (int i = 0; i < ROAD_SIZE; i++) {
        if (trafficZones[i] == zone && roadState[i] <= 2) {
            return roadState[i];
        }
    }
    return 0; // if unresolved return no traffic
}

bool isZoneLockedByEvent(char zone, int& forcedColor) {
    for (auto& ev : currentEvents) {
        if (ev.eventID == 1 && (zone == 'C' || zone == 'D' || zone == 'B')) { // Football match
            forcedColor = 2; // change to red/heavy traffic
            return true;
        }
         if (ev.eventID == 2 && (zone == 'K' || zone == 'A' || zone == 'B')) { // Mass
            forcedColor = 2; // change to red/heavy traffic
            return true;
        }
         if (ev.eventID == 3 && (zone == 'L')) { // Railway strike
            forcedColor = 2; // change to red/heavy traffic
            return true;
        }
         if (ev.eventID == 4 && (zone == 'E' || zone == 'D')) { // School
            forcedColor = 2; // change to red/heavy traffic
            return true;
        }
         if (ev.eventID == 5 && (zone == 'I' || zone == 'H')) { // Concert
            forcedColor = 2; // change to red/heavy traffic
            return true;
        }
    }
    return false;
}

void activateEvent(int id) {
    if (id == 1) { // Football match
        currentEvents.push_back({1, 4}); // ID 1, 3 rounds
        
        // changing traffic
        for(int i = 0; i < ROAD_SIZE; i++) {
            char z = trafficZones[i];
            if (z == 'C' || z == 'D' || z == 'B') {
                // Paint over roads, protecting barriers, passengers and players
                if (roadState[i] <= 2) { 
                    roadState[i] = 2;
                }
            }
        }
        cout << "\n[!] End of a football match! Zones C, D, B in heavy traffic for 3 rounds!" << endl;
    }
    if (id == 2) { // Mass
        currentEvents.push_back({2, 4}); // ID 2, 3 rounds
        
        // changing traffic
        for(int i = 0; i < ROAD_SIZE; i++) {
            char z = trafficZones[i];
            if (z == 'K' || z == 'A' || z == 'B') {
                // Paint over roads, protecting barriers, passengers and players
                if (roadState[i] <= 2) { 
                    roadState[i] = 2;
                }
            }
        }
        cout << "\n[!] Bells are ringing - Mass will begin shortly! Zones K, A, B in heavy traffic for 3 rounds!" << endl;
    }
    if (id == 3) { // Railway strike
        currentEvents.push_back({3, 4}); // ID 3, 3 rounds
        
        // changing traffic
        for(int i = 0; i < ROAD_SIZE; i++) {
            char z = trafficZones[i];
            if (z == 'L') {
                // Paint over roads, protecting barriers, passengers and players
                if (roadState[i] <= 2) { 
                    roadState[i] = 2;
                }
            }
        }
        cout << "\n[!] Railway workers are underpaid - Today is a day of a strike! Zones L in heavy traffic for 3 rounds!" << endl;
    }
    if (id == 4) { // School
        currentEvents.push_back({3, 4}); // ID 4, 3 rounds
        
        // changing traffic
        for(int i = 0; i < ROAD_SIZE; i++) {
            char z = trafficZones[i];
            if (z == 'E' || z =='D') {
                // Paint over roads, protecting barriers, passengers and players
                if (roadState[i] <= 2) { 
                    roadState[i] = 2;
                }
            }
        }
        cout << "\n[!] Today is the First day of school! Zones E, D in heavy traffic for 3 rounds!" << endl;
    }
    if (id == 5) { // Concert
        currentEvents.push_back({5, 4}); // ID 4, 3 rounds
        
        // changing traffic
        for(int i = 0; i < ROAD_SIZE; i++) {
            char z = trafficZones[i];
            if (z == 'I' || z =='H') {
                // Paint over roads, protecting barriers, passengers and players
                if (roadState[i] <= 2) { 
                    roadState[i] = 2;
                }
            }
        }
        cout << "\n[!] Taylor Swift is performing live at the city's biggest stadium! Zones I, H in heavy traffic for 3 rounds!" << endl;
    }
}

// Updating the events' lifespans
void updateEvents() {
    for (auto it = currentEvents.begin(); it != currentEvents.end(); ) {
        it->roundsLeft--;
        
        if (it->roundsLeft <= 0) {
            cout << "\n[!] Event (ID: " << it->eventID << ") has ended. Traffic comes back to normal." << endl;
            
            // manual change of traffic
            for(int i = 0; i < ROAD_SIZE; i++) {
                char z = trafficZones[i];
                bool shouldClear = false;

                // check what zones are involved
                if (it->eventID == 1 && (z == 'C' || z == 'D' || z == 'B')) shouldClear = true;
                if (it->eventID == 2 && (z == 'K' || z == 'A' || z == 'B')) shouldClear = true;
                if (it->eventID == 3 && (z == 'L')) shouldClear = true;
                if (it->eventID == 4 && (z == 'E' || z == 'D')) shouldClear = true;
                if (it->eventID == 5 && (z == 'I' || z == 'H')) shouldClear = true;

                // change red to green
                if (shouldClear && roadState[i] <= 2) {
                    roadState[i] = 0; 
                }
            }
            it = currentEvents.erase(it); 
        } else {
            ++it;
        }
    }
}

void updateTraffic(int currentRound) {

    // Updating every 3 rounds
    if ((currentRound - 1) % 3 == 0) {
        
        int newZoneState[256];
        
        // go through zones and set the traffic for every one of them
        for (char i = 'A'; i <= 'L'; i++) {
            // Special ROUND 1 logic - Generate immediate varied traffic
            if (currentRound == 1) {
                int chance = rand() % 100;
                switch (i) {
                    case 'A':
                    case 'B':
                    case 'G':
                    case 'H': // High traffic areas
                        newZoneState[i] = (chance < 25) ? 2 : ((chance < 75) ? 1 : 0);
                        break;
                    case 'D':
                    case 'E':
                    case 'J':
                    case 'L': // Medium traffic areas
                        newZoneState[i] = (chance < 10) ? 2 : ((chance < 45) ? 1 : 0);
                        break;
                    case 'C':
                    case 'F':
                    case 'I':
                    case 'K': // Almost No traffic areas
                        newZoneState[i] = (chance < 5) ? 2 : ((chance < 20) ? 1 : 0);
                        break;
                    default:
                        newZoneState[i] = (chance < 15) ? 2 : ((chance < 50) ? 1 : 0);
                        break;
                }
            } 
            // Logic for the rest of the game - smoothly transition colors based on their previous state
            else {
                int forcedColor = 0;
                // if locked by traffic keep the color
                if (isZoneLockedByEvent(i, forcedColor)) {
                    newZoneState[i] = forcedColor;
                } 
                // else calculate normal traffic
                else {
                    int currentState = getCurrentZoneState(i);
                    newZoneState[i] = getNextTrafficState(currentState, i); 
                } 
            }
        }

        for(int i = 0; i < ROAD_SIZE; i++) {

            if (roadState[i] == 5) {
                continue; // dont overwrite the barrier
            }

            if (roadState[i] == 3 && currentRound == 1) {
                continue; // if players are spawning skip
            }

            // Set the colors
            char currentZone = trafficZones[i];
            roadState[i] = newZoneState[currentZone];
        }
    } 
    else {
        for(int i = 0; i < ROAD_SIZE; i++) {
            if (roadState[i] == 4 || roadState[i] == 3) {
                // Overwrite spawn indicators with color from the zone
                char currentZone = trafficZones[i];
                roadState[i] = getCurrentZoneState(currentZone);
            }
        }
    }
}

void updateBarriers() {
    for (auto it = activeBarriers.begin(); it != activeBarriers.end(); ) {
        it->roundsLeft--; // rounds lifespan counter update
        
        if (it->roundsLeft <= 0) {
            // end of lifespan
            char zone = trafficZones[it->fieldIndex];
            roadState[it->fieldIndex] = getCurrentZoneState(zone);
            it = activeBarriers.erase(it);
            cout << "[!] Barrier on field " << it->fieldIndex + 1 << " is down!" << endl;

        } else {
            // preventing the cover up by traffic
            roadState[it->fieldIndex] = 5;
            ++it;
        }
    }
}
