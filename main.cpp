#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>

using namespace std;

const int PLAYERS_COUNT = 4;
const int ROAD_SIZE = 185;
const int PASSENGERS_AMOUNT = 21;

// index - index of a destination point(blue fields), value - index of the field on board
const vector<int> DESTINATIONS = {
    40, 44, 14, 4, 32, 48, 140, 70, 148, 23, 58, 81, 168, 110, 89, 125, 157, 163,
};

// Fields where player recieve Event cards are also their random spawn points
const vector<int> EVENTS = {
    9, 21, 36, 50, 75, 87, 103, 111, 161, 172,
};

int roadState[ROAD_SIZE];

struct PassengerCard {
    int cardID;
    int destination; 
    bool inPlay;
};

PassengerCard deck[PASSENGERS_AMOUNT];

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
        DESTINATIONS[0],  // Card 18 (duplicate)
        DESTINATIONS[1],  // Card 19 (duplicate)
        DESTINATIONS[2],  // Card 20 (duplicate)
        DESTINATIONS[3]   // Card 21 (duplicate)
    };

    for(int i = 0; i < PASSENGERS_AMOUNT; i++) {
        deck[i].cardID = i + 1; // Card numbers start from one
        deck[i].destination = predefinedDestinations[i];
        deck[i].inPlay = false;
    }
}


// GAME LOGIC
void initGame() {
    for(int i = 0; i < ROAD_SIZE; i++) {
        roadState[i] = 0; // 0 - empty road, 1 - slight traffic, 2 - heavy traffic, 3 - Player's Spawn Point, 4 - Passenger's Spawn Point
    } 
}

void spawnPassengers(int currentRound) {
    // TODO: CARDS AT SOME POINT, COME BACK TO THE DECK, GAME NEED TO KNOW WHEN CHANGE THE FLAG

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
            
            // Print
            cout << "[ROUND " << currentRound << "] Spawned card " 
                 << deck[randomCardIndex].cardID 
                 << " on field " << (spawnPos + 1)
                 << " (Dest: " << deck[randomCardIndex].destination << ")" << endl;
        }
    }
}

void spawnPlayers() {
    
    for (int i = 0; i < PLAYERS_COUNT; i++)
    {
        int spawnPos;

        int randSpawnPosID = rand() % EVENTS.size();
        spawnPos = EVENTS[randSpawnPosID] -1;
        
        // Update game
        roadState[spawnPos] = 3; // Mark spawned passenger on board
    }
}

void updateTraffic(int currentRound) {
    // TODO: Make traffic feel more natural, spawn in sectors, use intersections


    // Updating every 3 rounds
    if ((currentRound - 1) % 3 == 0) {
        
        for(int i = 0; i < ROAD_SIZE; i++) {

            if (roadState[i] == 3 && currentRound == 1) {
                continue; // if players are spawning skip
            }

            // random spawn chance for every field
            int spawnChance = rand() % 100; 
            
            if(spawnChance < 20) {
                roadState[i] = 2;      // 20% chance for a heavy traffic
            } 
            else if(spawnChance < 50) {
                roadState[i] = 1;      // 30% chance for a slight traffic
            } 
            else {
                roadState[i] = 0;     // 50% chance for an empty road
            }
        }
    } else {
        for(int i = 0; i < ROAD_SIZE; i++) {
            if (roadState[i] == 3 || roadState[i] == 4) {
                // Overwrite spawn indicators
                int spawnChance = rand() % 100; 
                
                if(spawnChance < 20) {
                    roadState[i] = 2;
                } 
                else if(spawnChance < 50) {
                    roadState[i] = 1;
                } 
                else {
                    roadState[i] = 0;
                }
            }
        }
    }
}

// TESTING ONLY
int SaveRoadStateToFile() {
    // Create and open the CSV file
    ofstream outFile("simulation.csv");
    if (!outFile.is_open()) {
        cout << "Error: Could not create simulation.csv file!" << endl;
        return 1;
    }

    int totalRounds = 50; // How many rounds to simulate
    cout << "Generating " << totalRounds << " rounds to simulation.csv..." << endl;

    for(int currentRound = 1; currentRound <= totalRounds; currentRound++) {

        if (currentRound == 1) {
            spawnPlayers();
        }
        updateTraffic(currentRound);
        spawnPassengers(currentRound);
        
        // Write the current road state to the file, separated by commas
        for(int i = 0; i < ROAD_SIZE; i++) {
            outFile << roadState[i];
            if (i < ROAD_SIZE - 1) outFile << ","; // Don't add comma after the last number
        }
        outFile << "\n"; // Next round goes to a new line
    }

    outFile.close();
    return 0;
}

int main() {
    srand(time(NULL)); 
    initGame();
    initPassengers();
    
    SaveRoadStateToFile();
    cout << "Done! You can now load simulation.csv into the HTML visualizer." << endl;
    
    return 0;
}