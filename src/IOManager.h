#pragma once

void initCSV();
int SaveRoadStateToFile();

void initArduino();   // Open + configure the serial port once at startup
void sendToArduino(); // Send the 12 zone states to the board
void closeArduino();  // Close the serial port at shutdown