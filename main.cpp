#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int PLAYERS_COUNT = 4;
const int ROAD_SIZE = 185;
const int PASSENGERS_AMOUNT = 21;
vector<int> mapGraph[ROAD_SIZE];

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

// Helper, adds two way connections - can create intersetions
void addTwoWay(int a, int b) {
    mapGraph[a - 1].push_back(b - 1);
    mapGraph[b - 1].push_back(a - 1);
}

// Helper, adds long paths of fields
void addSequence(int start, int end) {
    for (int i = start; i < end; i++) {
        addTwoWay(i, i + 1);
    }
}

void initGraph() {

    addSequence(1, 65);
    addSequence(66, 67);
    addSequence(68, 125);
    addSequence(126, 126);
    addSequence(127, 128);
    addSequence(129, 131);
    addSequence(132, 137);
    addSequence(138, 140);
    addSequence(141, 149);
    addSequence(150, 155);
    addSequence(156, 163);
    addSequence(164, 166);
    addSequence(167, 174);
    addSequence(175, 176);
    addSequence(177, 181);
    addSequence(182, 185);


    addTwoWay(16, 137);
    addTwoWay(16, 138);
    
    addTwoWay(19, 67);
    addTwoWay(19, 68);

    addTwoWay(25, 65);
    addTwoWay(25, 66);

    addTwoWay(27, 141);
    addTwoWay(27, 140);

    addTwoWay(34, 185);

    addTwoWay(45, 182);
    addTwoWay(45, 181);

    addTwoWay(57, 150);
    addTwoWay(57, 149);

    addTwoWay(63, 175);
    addTwoWay(63, 174);

    addTwoWay(72, 131);
    addTwoWay(72, 132);

    addTwoWay(80, 128);
    addTwoWay(80, 129);

    addTwoWay(86, 126);
    addTwoWay(86, 127);

    addTwoWay(92, 125);
    addTwoWay(92, 126);

    addTwoWay(98, 166);
    addTwoWay(98, 167);

    addTwoWay(114, 156);
    addTwoWay(114, 155);

    addTwoWay(118, 163);
    addTwoWay(118, 164);

    addTwoWay(135, 1);

    addTwoWay(145, 177);
    addTwoWay(145, 176);

    addTwoWay(153, 102);
    addTwoWay(153, 101);
}

void printGraph() {
    cout << "--- VERIFYING MAP CONNECTIONS ---" << endl;
    
    for (int i = 0; i < ROAD_SIZE; i++) {

        cout << "Field " << (i + 1) << " connects to: [ ";
        
        // Loop through all connections for this specific field
        for (size_t j = 0; j < mapGraph[i].size(); j++) {
            
            // Add 1 to the neighbor's index to match the physical map
            cout << (mapGraph[i][j] + 1); 
            
            // Add a comma between numbers but not after the last one
            if (j < mapGraph[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << " ]" << endl;
    }
    cout << "---------------------------------" << endl;
}

// BFS for calculating the shortest path to destination point
// used to calcualte the taxi rate
// using 0-based indices (0 to 184)
vector<int> findShortestPath(int startIndex, int destinationIndex) {

    vector<int> parent(ROAD_SIZE, -1); // Keeps track of where we came from
    vector<bool> visited(ROAD_SIZE, false); // Keeps track of already checked fields 
    queue<int> q; // The BFS queue

    // 2. Start the BFS with the beginning field
    q.push(startIndex);
    visited[startIndex] = true;

    bool found = false;

    // Run the search
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // If reached the destination, stop searching
        if (current == destinationIndex) {
            found = true;
            break; 
        }

        // Check all neighboring fields
        for (size_t i = 0; i < mapGraph[current].size(); i++) {
            int neighbor = mapGraph[current][i];

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // Flip the path
    vector<int> path;
    if (found) {
        int step = destinationIndex;
        while (step != -1) {
            path.push_back(step);
            step = parent[step];
        }
        // The path is currently backwards (Destination -> Start)
        reverse(path.begin(), path.end());
    }

    // Returns the path, empty if no path exists (on this board a path always exists)
    return path;
}


int calcFare(int distance) {
    //  Base Fare + (Distance x Rate Per Field)

    if (distance <= 0) return 0;

    float baseFare = 2.0f;
    float ratePerField = 0.5f;

    float total = baseFare + (distance * ratePerField);

    return (int)total;
}

void initGame() {
    for(int i = 0; i < ROAD_SIZE; i++) {
    // 0 - empty road,
    // 1 - slight traffic
    // 2 - heavy traffic
    // 3 - Player's spawn point
    // 4 - Passenger's spawn point
        roadState[i] = 0;
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
            
            vector<int> path = findShortestPath(spawnPos, deck[randomCardIndex].destination);
            int dst = path.size();

            int fare = calcFare(dst);

            // Print
            cout << "[ROUND " << currentRound << "] Spawned card " 
                 << deck[randomCardIndex].cardID 
                 << " on field " << (spawnPos + 1)
                 << " (Dest: " << deck[randomCardIndex].destination << ")"
                 << " - Dst: " << dst
                 << " | Fare: " << fare
                << endl;
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
    initGraph();      
    printGraph();
    initPassengers();
    
    SaveRoadStateToFile();
    cout << "Done! You can now load simulation.csv into the HTML visualizer." << endl;
    
    return 0;
}