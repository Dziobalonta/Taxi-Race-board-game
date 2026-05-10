#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// Road size
const int ROAD_SIZE = 200;
int roadState[ROAD_SIZE];

// GAME LOGIC
void initGame() {
    for(int i = 0; i < ROAD_SIZE; i++) {
        roadState[i] = 0; // 0 - empty road, 1 - slight traffic, 2 - heavy traffic
    }
}

void updateTraffic() {
    // 1. Move all vehicles to the right (from the end to avoid overwriting)
    for(int i = ROAD_SIZE - 1; i > 0; i--) {
        roadState[i] = roadState[i - 1];
    }
    
    // 2. Spawn new traffic at the beginning of the road
    int spawnChance = rand() % 100; 
    if(spawnChance < 20) {
        roadState[0] = 2;      // 20% chance for a heavy traffic
    } 
    else if(spawnChance < 50) {
        roadState[0] = 1;      // 30% chance for a slight traffic
    } 
    else {
        roadState[0] = 0;      // 50% chance for an empty road
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

    int totalRounds = 50; // You can change how many rounds to simulate
    cout << "Generating " << totalRounds << " rounds to simulation.csv..." << endl;

    for(int currentRound = 1; currentRound <= totalRounds; currentRound++) {
        updateTraffic();
        
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
    
    SaveRoadStateToFile();
    cout << "Done! You can now load simulation.csv into the HTML visualizer." << endl;
    
    return 0;
}