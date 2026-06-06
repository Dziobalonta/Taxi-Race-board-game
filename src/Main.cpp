
#include "Config.h"
#include "MapGraph.h"
#include "GameLogic.h"
#include "Entities.h"
#include "IOManager.h"

#include <iostream>
#include "conio_compat.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int main() {
    srand(time(NULL)); 
    if (useArduino) initArduino();

    sleep(1);
    initGame();
    initZones();
    initGraph();     
    // printGraph();
    initPassengers();
    initCSV();

    int currentRound = 1;
    bool gameRunning = true;

    system(CLEAR_SCREEN);

    cout << "--- GAME STARTED ---" << endl;
    cout << "----- ROUND 1 -----" << endl;
    cout << "Open your HTML Visualizer now. It will update as you play." << endl;

    cout << endl;   
    cout << "Press [SPACE] or [ENTER] for next round, [Q] to quit." << endl;
    cout << "--------------------" << endl;
    cout << endl;

    // First Round Setup
    spawnPlayers();
    updateTraffic(currentRound);
    spawnPassengers(currentRound);
    SaveRoadStateToFile();

    // THE CLI MENU
    while (gameRunning) {
        cout << "\n--------------------" << endl;
        // EVENTS
        cout << "What event has been activated?" << endl;
        cout << "[1] End of a football match" << endl;
        cout << "[2] Mass in the church" << endl;
        cout << "[3] Railway workers' strike" << endl;
        cout << "[4] First day of school" << endl;
        cout << "[5] Concert of a world-famous artist at the stadium" << endl;

        cout << "[SPACE] Skip" << endl;
        cout << "> Select event (1-5 or [SPACE]): ";

        char eventKey = _getch();
        if (eventKey >= '1' && eventKey <= '5') {
            int eventID = eventKey - '0'; // conversion
            activateEvent(eventID);
        } else if (eventKey == 'q' || eventKey == 'Q') {
            gameRunning = false;
            break;
        } else {
            cout << "\n> No Events this round..." << endl;
        }

        // BARRIERS
        cout << "\n> Place a barrier? \n(Type field number + [ENTER] or [SPACE] to skip): ";
        char key = _getch(); // Catches the first key press

        // QUIT GAME
        if (key == 'q' || key == 'Q') { 
            gameRunning = false;
            cout << "\nEnding game..." << endl;
            break;
        }

        int barrierField = -1;

        // checking input
        if (key == ' ') {
            // Space - skip
        } 
        else if (key >= '0' && key <= '9') {
            cout << key; // Print the first pressed digit
            string rest;
            getline(cin, rest); // Read the remaining digits until Enter is pressed
            
            try {
                barrierField = stoi(string(1, key) + rest);
                if (barrierField < 1 || barrierField > ROAD_SIZE) {
                    cout << "> Invalid field number! Skipping barrier." << endl;
                    barrierField = -1;
                }
            } catch (...) {
                cout << "> Input error! Skipping." << endl;
                barrierField = -1;
            }
        } 
        else {
            cout << "\n> Unrecognized key. Skipping barrier." << endl;
        }

        // NEW ROUND
        currentRound++;
        system(CLEAR_SCREEN);

        cout << "--- GAME RUNNING ---" << endl;
        cout << "----- ROUND " << currentRound <<" -----" << endl;
        cout << endl;

        // Remove outdated things from map
        updateEvents();
        updateBarriers();

        // If a valid number was provided - place a new roadblock for 3 rounds
        if (barrierField != -1) {
            activeBarriers.push_back({barrierField - 1, 3});
            roadState[barrierField - 1] = 5;
            cout << "[!] Barrier placed on field " << barrierField << " for 3 rounds!\n" << endl;
        }

        updateTraffic(currentRound);
        spawnPassengers(currentRound);
        
        SaveRoadStateToFile();

        if(useArduino){
            sendToArduino();
        }
    }

    if (useArduino) closeArduino();

    return 0;
}