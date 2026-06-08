#include "IOManager.h"
#include "Config.h"
#include "GameLogic.h"

#include <iostream>
#include <fstream>
#include <string>

#ifndef _WIN32
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

// LED map: board field (1-191) -> physical LED index(es) on the strip.
// Moved here from the Arduino sketch so the board only receives final LED
// indices and no longer needs the table itself.
struct FieldLeds {
    int count;
    int leds[3]; // a field maps to at most 3 LEDs
};

static const FieldLeds boardFields[ROAD_SIZE] = {
    {1, {1}},                  // Pole 1
    {1, {2}},                  // Pole 2
    {1, {3}},                  // Pole 3
    {1, {4}},                  // Pole 4
    {1, {5}},                  // Pole 5
    {1, {6}},                  // Pole 6
    {1, {7}},                  // Pole 7
    {1, {7}},                  // Pole 8
    {1, {8}},                  // Pole 9
    {1, {9}},                  // Pole 10
    {1, {9}},                  // Pole 11
    {1, {10}},                 // Pole 12
    {1, {12}},                 // Pole 13
    {1, {13}},                 // Pole 14
    {1, {14}},                 // Pole 15
    {1, {14}},                   // Pole 16
    {1, {30}},                 // Pole 17
    {2, {28, 29}},             // Pole 18
    {1, {29}},                   // Pole 19
    {1, {226}},                   // Pole 20
    {1, {226}},                   // Pole 21
    {1, {225}},                // Pole 22
    {1, {225}},                // Pole 23
    {1, {223}},                   // Pole 24
    {1, {223}},                // Pole 25
    {1, {230}},                // Pole 26
    {1, {211}},                   // Pole 27
    {2, {209, 210}},           // Pole 28
    {1, {208}},                // Pole 29
    {1, {207}},                // Pole 30
    {1, {206}},                // Pole 31
    {1, {205}},                // Pole 32
    {1, {204}},                   // Pole 33
    {1, {43}},                   // Pole 34
    {1, {44}},                 // Pole 35
    {1, {45}},                 // Pole 36
    {2, {46, 47}},             // Pole 37
    {1, {48}},                 // Pole 38
    {1, {49}},                 // Pole 39
    {1, {49}},                 // Pole 40
    {1, {50}},                 // Pole 41
    {2, {51, 52}},             // Pole 42
    {1, {53}},                 // Pole 43
    {1, {54}},                 // Pole 44
    {2, {55, 56}},             // Pole 45
    {1, {57}},                 // Pole 46
    {1, {58}},                   // Pole 47
    {2, {59, 60}},             // Pole 48
    {1, {60}},                   // Pole 49
    {2, {61, 62}},             // Pole 50
    {1, {63}},                   // Pole 51
    {1, {64}},                   // Pole 52
    {1, {65}},                   // Pole 53
    {1, {66}},                 // Pole 54
    {1, {67}},                   // Pole 55
    {1, {68}},                   // Pole 56
    {1, {70}},                   // Pole 57
    {1, {186}},                // Pole 58
    {2, {183, 184}},           // Pole 59
    {1, {182}},                // Pole 60
    {1, {181}},                // Pole 61
    {1, {180}},                // Pole 62
    {1, {179}},                   // Pole 63
    {1, {220}},                   // Pole 64
    {1, {221}},                // Pole 65
    {1, {228}},                // Pole 66
    {1, {227}},                   // Pole 67
    {2, {26, 27}},             // Pole 68
    {1, {25}},                 // Pole 69
    {1, {24}},                 // Pole 70
    {1, {23}},                 // Pole 71
    {1, {22}},                   // Pole 72
    {1, {149}},                // Pole 73
    {1, {149}},                // Pole 74
    {1, {148}},                // Pole 75
    {1, {147}},                // Pole 76
    {1, {146}},                // Pole 77
    {1, {145}},                // Pole 78
    {1, {144}},                // Pole 79
    {1, {154}},                   // Pole 80
    {1, {155}},                // Pole 81
    {1, {156}},                // Pole 82
    {1, {157}},                // Pole 83
    {2, {158, 159}},           // Pole 84
    {1, {160}},                   // Pole 85
    {1, {137}},                // Pole 86
    {1, {136}},                // Pole 87
    {1, {135}},                // Pole 88
    {1, {135}},                // Pole 89
    {1, {134}},                // Pole 90
    {1, {134}},                // Pole 91
    {1, {132}},                // Pole 92
    {1, {162}},                   // Pole 93
    {2, {163, 164}},           // Pole 94
    {2, {165, 166}},           // Pole 95
    {1, {167}},                   // Pole 96
    {1, {168}},                   // Pole 97
    {1, {169}},                   // Pole 98
    {2, {114, 115}},           // Pole 99
    {1, {113}},                // Pole 100
    {1, {112}},                // Pole 101
    {1, {77}},                 // Pole 102
    {1, {76}},                   // Pole 103
    {1, {80}},                 // Pole 104
    {2, {81, 82}},             // Pole 105
    {1, {83}},                   // Pole 106
    {1, {84}},                 // Pole 107
    {1, {85}},                 // Pole 108
    {1, {86}},                 // Pole 109
    {1, {87}},                 // Pole 110
    {1, {89}},                   // Pole 111
    {1, {90}},                 // Pole 112
    {2, {91, 92}},             // Pole 113
    {1, {93}},                 // Pole 114
    {3, {104, 105, 106}},      // Pole 115
    {1, {103}},                // Pole 116
    {1, {102}},                // Pole 117
    {1, {101}},                   // Pole 118
    {1, {123}},                // Pole 119
    {1, {124}},                // Pole 120
    {1, {125}},                // Pole 121
    {1, {127}},                // Pole 122
    {1, {128}},                // Pole 123
    {1, {129}},                   // Pole 124
    {1, {130}},                   // Pole 125
    {1, {161}},                   // Pole 126
    {1, {140}},                // Pole 127
    {1, {141}},                // Pole 128
    {2, {153, 154}},           // Pole 129
    {1, {152}},                // Pole 130
    {1, {151}},                   // Pole 131
    {1, {21}},                 // Pole 132
    {1, {20}},                 // Pole 133
    {1, {19}},                 // Pole 134
    {1, {16}},                 // Pole 135
    {1, {15}},                 // Pole 136
    {1, {14}},                   // Pole 137
    {1, {31}},                 // Pole 138
    {2, {32, 33}},             // Pole 139
    {2, {232, 233}},           // Pole 140
    {1, {212}},                // Pole 141
    {1, {214}},                // Pole 142
    {1, {215}},                // Pole 143
    {1, {216}},                // Pole 144
    {1, {193}},                   // Pole 145
    {1, {192}},                // Pole 146
    {1, {191}},                   // Pole 147
    {1, {190}},                // Pole 148
    {1, {188}},                // Pole 149
    {1, {71}},                 // Pole 150
    {1, {72}},                 // Pole 151
    {2, {73, 74}},             // Pole 152
    {1, {76}},                 // Pole 153
    {1, {108}},                // Pole 154
    {1, {107}},                // Pole 155
    {1, {95}},                   // Pole 156
    {1, {96}},                 // Pole 157
    {1, {97}},                 // Pole 158
    {2, {97, 98}},             // Pole 159
    {1, {98}},                 // Pole 160
    {2, {99, 100}},            // Pole 161
    {1, {101}},                   // Pole 162
    {1, {102}},                   // Pole 163
    {1, {122}},                // Pole 164
    {1, {121}},                // Pole 165
    {1, {120}},                   // Pole 166
    {1, {169}},                   // Pole 167
    {1, {170}},                // Pole 168
    {2, {171, 172}},           // Pole 169
    {1, {173}},                // Pole 170
    {2, {174, 175}},           // Pole 171
    {1, {176}},                // Pole 172
    {1, {177}},                // Pole 173
    {1, {178}},                // Pole 174
    {1, {217}},                // Pole 175
    {1, {216}},                   // Pole 176
    {1, {194}},                // Pole 177
    {1, {195}},                   // Pole 178
    {1, {196}},                // Pole 179
    {1, {197}},                // Pole 180
    {1, {198}},                   // Pole 181
    {1, {200}},                   // Pole 182
    {2, {201, 202}},           // Pole 183
    {1, {203}},                // Pole 184
    {2, {40, 41}},             // Pole 185
    {1, {39}},                 // Pole 186
    {1, {38}},                 // Pole 187
    {1, {37}},                 // Pole 188
    {1, {36}},                 // Pole 189
    {1, {35}},                 // Pole 190
    {1, {34}}                  // Pole 191
};

// Append the LED indices of every field whose roadState is s1 or s2 to `frame`
// as ",<led>" tokens. Pass s2 = -1 to match a single state.
static void appendLedsForStates(string& frame, int s1, int s2) {
    for (int f = 0; f < ROAD_SIZE; f++) {
        if (roadState[f] == s1 || (s2 >= 0 && roadState[f] == s2)) {
            const FieldLeds& fl = boardFields[f];
            for (int j = 0; j < fl.count; j++) {
                frame += ',';
                frame += to_string(fl.leds[j]);
            }
        }
    }
}

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
    // Frame: 12 zone states + spawn-LED indices + ';' + barrier-LED indices + '\n'
    // e.g. "001020010200,8,61,62;45,90\n"
    //  - spawns (roadState 3/4) -> blinking white on the board
    //  - barriers (roadState 5) -> solid color on the board
    string frame;
    for (char z = 'A'; z <= 'L'; z++) {
        frame += char('0' + getCurrentZoneState(z));
    }
    appendLedsForStates(frame, 3, 4); // spawns
    frame += ';';
    appendLedsForStates(frame, 5, -1); // barriers
    frame += '\n';
    write(g_arduinoFd, frame.c_str(), frame.size());
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
        // 12 zone states + spawn LEDs + ';' + barrier LEDs + '\n'.
        string frame;
        for (char z = 'A'; z <= 'L'; z++) {
            frame += char('0' + getCurrentZoneState(z));
        }
        appendLedsForStates(frame, 3, 4); // spawns
        frame += ';';
        appendLedsForStates(frame, 5, -1); // barriers
        frame += '\n';
        arduino << frame;
        arduino.close();
    } else {
        cout << "[!] Cannot connect to Arduino on port " << ARDUINO_PORT << "!" << endl;
    }
}

void closeArduino() {}
#endif