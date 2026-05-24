#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <conio.h>

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

char trafficZones[ROAD_SIZE];

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
    setZoneFromList({ 23, 22, 21, 20, 19, 18, 17, 16, 15, 67, 68, 139, 138, 137 }, 'B');
    setZoneFromList({ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 'C');
    setZoneFromList({ 136, 135, 134, 133, 132, 131, 130, 69, 70, 71, 72, 73, 74, 75, 76, 1 }, 'D');
    setZoneFromList({ 129, 128, 127, 126, 125, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93 }, 'E');
    setZoneFromList({ 96, 95, 94, 124, 123, 122, 121, 120, 173, 172, 171, 170, 169, 168 }, 'F');
    setZoneFromList({ 167, 166, 165, 164, 163, 162, 161, 97, 98, 99, 100, 119, 118, 117, 116 }, 'G');
    setZoneFromList({ 152, 153, 154, 155, 156, 157, 158, 159, 160, 113, 114, 115, 102, 101 }, 'H');
    setZoneFromList({ 103, 104, 105, 106, 107, 108, 109, 110, 111, 112 }, 'I');
    setZoneFromList({ 54, 55, 56, 57, 58, 59, 60, 147, 148, 149, 150, 151 }, 'J');
    setZoneFromList({ 29, 30, 31, 32, 178, 179, 180, 53, 52, 51, 50, 49, 48, 47 }, 'K');
    setZoneFromList({ 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 181, 182, 183, 184, 185 }, 'L');
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
            // cout << "[ROUND " << currentRound << "] Spawned card "
            cout << "Spawned card " 
                 << deck[randomCardIndex].cardID 
                 << " on field " << (spawnPos + 1)
                 << " (Dest: " << deck[randomCardIndex].destination << ")"
                 << " - Dst: " << dst
                 << " | Fare: " << fare << " zl"
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

bool isJunction(int fieldIndex) {
    return mapGraph[fieldIndex].size() > 2;
}

bool isNearJunction(int fieldIndex) {
    // Look at all adjacent fields
    for (size_t i = 0; i < mapGraph[fieldIndex].size(); i++) {
        int neighbor = mapGraph[fieldIndex][i];
        if (isJunction(neighbor)) {
            return true;
        }
    }
    return false;
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
                int currentState = getCurrentZoneState(i);
                newZoneState[i] = getNextTrafficState(currentState, i); 
            }
        }

        for(int i = 0; i < ROAD_SIZE; i++) {

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

// TESTING ONLY
int SaveRoadStateToFile() {
    // 'app' mode appends to the end of the file instead of overwriting it
    ofstream outFile("BoardState.csv", ios::app); 
    
    for(int i = 0; i < ROAD_SIZE; i++) {
        outFile << roadState[i];
        if (i < ROAD_SIZE - 1) outFile << ","; 
    }
    outFile << "\n"; 
    
    outFile.close();

    return 0;
}

void initCSV() {
    ofstream outFile("BoardState.csv", ios::trunc); // 'trunc' deletes old content
    if (!outFile.is_open()) {
        cout << "Error: Could not create simulation.csv file!" << endl;
    }
    outFile.close();
}

int main() {
    srand(time(NULL)); 
    initGame();
    initZones();
    initGraph();      
    // printGraph();
    initPassengers();
    initCSV();

    int currentRound = 1;
    bool gameRunning = true;

    system("cls");

    cout << "--- GAME STARTED ---" << endl;
    cout << "----- ROUND 1 -----" << endl;
    cout << "Open your HTML Visualizer now. It will update as you play." << endl;
    cout << "--------------------" << endl;
    cout << endl;

    // First Round Setup
    spawnPlayers();
    updateTraffic(currentRound);
    spawnPassengers(currentRound);
    SaveRoadStateToFile();

    cout << endl;   
    cout << "--------------------" << endl;
    cout << "Press [SPACE] or [ENTER] for next round, [Q] to quit." << endl;

    // THE CLI MENU
    while (gameRunning) {
        // _getch() waits for ANY key press and immediately moves forward
        char key = _getch(); 

        // 1. QUIT GAME
        if (key == 'q' || key == 'Q') { 
            gameRunning = false;
            cout << "\nEnding game..." << endl;
        } 
        // 2. NEXT ROUND (Space is 32 or ' ', Enter is 13)
        else if (key == ' ' || key == 13) { 
            currentRound++;

            system("cls");

            cout << "--- GAME RUNNING ---" << endl;
            cout << "----- ROUND " << currentRound <<" -----" << endl;
            cout << endl;
            
            
            // Advance the game logic
            updateTraffic(currentRound);
            spawnPassengers(currentRound);
            
            // Save the new state
            SaveRoadStateToFile();

            cout << endl;
            cout << "--------------------" << endl;
            cout << "Press [SPACE] or [ENTER] for next round, [Q] to quit." << endl;
        } 
        // 3. EXAMPLE OF ADDING MORE COMMANDS LATER
        else if (key == 'p' || key == 'P') {
            cout << "\nYou pressed P! Later, you can put 'pick up passenger' logic here." << endl;
        }
        else if (key == 'h' || key == 'H') {
            cout << "\nHELP: Press Space for next round, Q to quit." << endl;
        }
        // If they press an unmapped key, the loop just ignores it and waits for the next press
    }
    
    return 0;
}