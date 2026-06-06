#include "IOManager.h"
#include "Config.h"
#include "GameLogic.h"

#include <iostream>
#include <fstream>

using namespace std;

void initCSV() {
    ofstream outFile("BoardState.csv", ios::trunc); // 'trunc' deletes old content
    if (!outFile.is_open()) {
        cout << "Error: Could not create BoardState.csv file!" << endl;
    }
    outFile.close();
}

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

void sendToArduino() {
    // Check in the Arduino IDE which COM port your Arduino is using!
    // The format for ports above COM9 is e.g., "\\\\.\\COM10", for lower ones "COM3" is enough
    ofstream arduino("COM5");
    
    if (arduino.is_open()) {
        // Iterate through all 12 zones (A-L)
        for (char z = 'A'; z <= 'L'; z++) {
            int state = getCurrentZoneState(z); // Get the current traffic state in the zone
            arduino << state; // Send the digit to Arduino (e.g., '2')
        }

        for (int i = 0; i < ROAD_SIZE; i++)
        {
            arduino << roadState[i];
        }
        // char that tells to stop reading
        arduino << '\n';
        
        arduino.close(); // Close the connection
    } else {
        cout << "[!] Cannot connect to Arduino on port COM5!" << endl;
    }
}