#include <iostream>
#include <cstdlib>
#include <ctime>

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
        roadState[0] = 2;      // 20% chance for a traffic jam (X)
    } 
    else if(spawnChance < 50) {
        roadState[0] = 1;      // 30% chance for a moving car (O)
    } 
    else {
        roadState[0] = 0;      // 50% chance for an empty road (_)
    }
}

// TESTING ONLY
void printRoad() {
    for(int i = 0; i < ROAD_SIZE; i++) {
        if(roadState[i] == 0) cout << "_ ";
        else if(roadState[i] == 1) cout << "O ";
        else if(roadState[i] == 2) cout << "X ";
    }
    cout << endl;
}

int main() {
    srand(time(NULL)); // Initialize random seed
    
    initGame();
    cout << "GAME START:" << endl;
    printRoad();
    cout << "------------------------------------------------------------" << endl;
    cout << "Press ENTER to simulate the next round" << endl;
    
    int currentRound = 1;
    
    while(true) {
        cin.get(); 
        
        cout << "Round " << currentRound << ":\t";
        updateTraffic();
        printRoad();
        
        currentRound++;
    }
    
    return 0;
}