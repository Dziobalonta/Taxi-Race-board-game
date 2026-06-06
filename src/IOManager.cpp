#include "IOManager.h"
#include "Config.h"
#include "GameLogic.h"

#include <iostream>
#include <fstream>

#ifndef _WIN32
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

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

#ifndef _WIN32
// ---- macOS / Linux: configure the serial port properly and keep it open ----
// Opening a cu.usbmodem port toggles DTR and resets the Arduino, so we open it
// ONCE at startup, wait for the board to boot, then reuse the fd every round.
static int g_arduinoFd = -1;

void initArduino() {
    if (!useArduino) return;

    g_arduinoFd = open(ARDUINO_PORT, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (g_arduinoFd < 0) {
        cout << "[!] Cannot open Arduino on port " << ARDUINO_PORT << "!" << endl;
        return;
    }

    struct termios tty;
    tcgetattr(g_arduinoFd, &tty);
    cfsetispeed(&tty, B9600);          // must match Serial.begin(9600) in the sketch
    cfsetospeed(&tty, B9600);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 data bits
    tty.c_cflag &= ~PARENB;            // no parity
    tty.c_cflag &= ~CSTOPB;            // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;           // no hardware flow control
    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem ctrl lines, enable receiver
    cfmakeraw(&tty);                   // raw mode: send bytes verbatim
    tcsetattr(g_arduinoFd, TCSANOW, &tty);

    fcntl(g_arduinoFd, F_SETFL, 0);    // back to blocking writes

    sleep(2);                          // give the Arduino time to reboot after the reset
    tcflush(g_arduinoFd, TCIOFLUSH);   // discard any boot noise
    cout << "[*] Arduino connected on " << ARDUINO_PORT << endl;
}

void sendToArduino() {
    if (g_arduinoFd < 0) {
        cout << "[!] Arduino not connected on port " << ARDUINO_PORT << "!" << endl;
        return;
    }
    char buf[12];
    int i = 0;
    for (char z = 'A'; z <= 'L'; z++) {
        buf[i++] = '0' + getCurrentZoneState(z); // send the state as an ASCII digit
    }
    write(g_arduinoFd, buf, sizeof(buf));
    tcdrain(g_arduinoFd); // block until the bytes are physically sent
}

void closeArduino() {
    if (g_arduinoFd >= 0) {
        close(g_arduinoFd);
        g_arduinoFd = -1;
    }
}

#else
// ---- Windows: the simple ofstream approach works as before ----
void initArduino() {}

void sendToArduino() {
    ofstream arduino(ARDUINO_PORT);
    if (arduino.is_open()) {
        for (char z = 'A'; z <= 'L'; z++) {
            int state = getCurrentZoneState(z);
            arduino << state;
        }
        arduino.close();
    } else {
        cout << "[!] Cannot connect to Arduino on port " << ARDUINO_PORT << "!" << endl;
    }
}

void closeArduino() {}
#endif