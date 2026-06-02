#include "Entities.h"
#include "Config.h"
#include "MapGraph.h"

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

void initPassengers() {
    int predefinedDestinations[PASSENGERS_AMOUNT] = {
        DESTINATIONS[0],  // Card 1
        DESTINATIONS[1],  // Card 2
        DESTINATIONS[2],  // Card 3
        DESTINATIONS[3],  // Card 4
        DESTINATIONS[4],  // Card 5
        DESTINATIONS[5],  // Card 6
        DESTINATIONS[6],  // Card 7
        DESTINATIONS[7],  // Card 8
        DESTINATIONS[8],  // Card 9
        DESTINATIONS[9],  // Card 10
        DESTINATIONS[10], // Card 11
        DESTINATIONS[11], // Card 12
        DESTINATIONS[12], // Card 13
        DESTINATIONS[13], // Card 14
        DESTINATIONS[14], // Card 15
        DESTINATIONS[15], // Card 16
        DESTINATIONS[16], // Card 17
        DESTINATIONS[17], // Card 18
        DESTINATIONS[0],  // Card 19 (duplicate)
        DESTINATIONS[1],  // Card 20 (duplicate)
        DESTINATIONS[2]   // Card 21 (duplicate)
    };

    for(int i = 0; i < PASSENGERS_AMOUNT; i++) {
        deck[i].cardID = i + 1; // Card numbers start from one
        deck[i].destination = predefinedDestinations[i];
        deck[i].inPlay = false;
    }
}

int calcFare(int distance) {
    //  Base Fare + (Distance x Rate Per Field)
    if (distance <= 0) return 0;

    float baseFare = 2.0f;
    float ratePerField = 0.5f;

    float total = baseFare + (distance * ratePerField);

    return (int)total;
}

void spawnPassengers(int currentRound) {
    // Spawning every 3 rounds
    if ((currentRound - 1) % 3 == 0) {
        
        int newPassengersThisRound = 4; 
        
        for (int p = 0; p < newPassengersThisRound; p++) {
            
            // Find random card thats is not on board
            int randomCardIndex;
            bool cardFound = false;
            int attempts = 0;
            
            do {
                randomCardIndex = rand() % PASSENGERS_AMOUNT;
                if (!deck[randomCardIndex].inPlay) {
                    cardFound = true;
                }
                attempts++;
                if (attempts > 50) break; // fail safe if all cards are in game
            } while (!cardFound);
            
            if (!cardFound) continue; // no card in deck
            
            int spawnPos;
            bool validPos = false;
            
            // Find field to spawn a passenger
            do {
                spawnPos = rand() % ROAD_SIZE;
                
                // check if there are no other spawnpoints for smth
                if (roadState[spawnPos] == 0  || roadState[spawnPos] == 1 || roadState[spawnPos] == 2) {
                    validPos = true;
                }
            } while (!validPos);
            
            // Update game
            roadState[spawnPos] = 4; // Mark spawned passenger on board
            deck[randomCardIndex].inPlay = true; // card out of the deck
            
            vector<int> path = findShortestPath(spawnPos, deck[randomCardIndex].destination);
            int dst = path.size();

            int fare = calcFare(dst);

            // Print
            cout << " [!] Spawned card " 
                 << deck[randomCardIndex].cardID 
                 << " on field " << (spawnPos + 1)
                 << " (Dest: " << deck[randomCardIndex].destination << ")"
                //  << " - Dst: " << dst
                 << " | Fare: " << fare << " zl"
                 << endl;
        }
    }
}

void spawnPlayers() {
    for (int i = 0; i < PLAYERS_COUNT; i++) {
        int spawnPos;

        int randSpawnPosID = rand() % EVENTS.size();
        spawnPos = EVENTS[randSpawnPosID] - 1;
        
        // Update game
        roadState[spawnPos] = 3; // Mark spawned passenger on board
    }
}