#include <FastLED.h>

#define LED_PIN     6          // Zmień na numer pinu, do którego podpięty jest pasek
#define NUM_LEDS    234        // Całkowita liczba LED-ów na pasku (zaktualizowana do Twojej mapy)
#define LED_TYPE    WS2812B    // Zmień typ, jeśli masz np. WS2811 lub SK6812
#define COLOR_ORDER GRB        // Kolejność kolorów (najczęściej GRB dla WS2812B)

CRGB leds[NUM_LEDS];

// --- Definicje stref (kolekcje LED-ów zaktualizowane przez Ciebie) ---
const uint8_t ledsA[] = {178,179, 180, 181,192, 193, 194, 209,210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,228, 229, 230, 231, 232, 233};
const uint8_t ledsB[] = {13, 14, 15, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 224,225, 226, 227};
const uint8_t ledsC[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const uint8_t ledsD[] = {1, 2, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 147,148, 149, 150, 151, 152};
const uint8_t ledsE[] = {130,131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 153,154, 155, 156, 157, 158, 159, 160, 161, 162, 163};
const uint8_t ledsF[] = {125,126, 127, 128, 129, 164,165, 166, 171,172, 173, 174, 175, 176, 177};
const uint8_t ledsG[] = {99, 100, 101, 102, 103, 114,115, 116, 117, 118, 119, 120, 121, 122, 123, 124,167, 168, 169, 170};
const uint8_t ledsH[] = {72,73, 74, 75, 76, 77, 78, 80,91, 92, 93, 94, 95, 96, 97, 98, 104,105, 106, 107, 108, 109, 110, 111, 112, 113};
const uint8_t ledsI[] = {79,80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
const uint8_t ledsJ[] = {67, 68, 69, 70, 71,182, 183, 184, 185, 186, 187, 188, 189, 190, 191};
const uint8_t ledsK[] = {36, 37, 38, 39, 59,60, 61, 62, 63, 64, 65, 66, 195,196, 197, 206,207, 208};
const uint8_t ledsL[] = {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,198, 199, 200, 201, 202, 203, 204, 205};

// Tworzymy główną tablicę ze wszystkimi 191 polami
const Field boardFields[191] = {
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
    {0, {}},                   // Pole 16
    {1, {30}},                 // Pole 17
    {2, {28, 29}},             // Pole 18
    {0, {}},                   // Pole 19
    {0, {}},                   // Pole 20
    {0, {}},                   // Pole 21
    {1, {225}},                // Pole 22
    {1, {225}},                // Pole 23
    {0, {}},                   // Pole 24
    {1, {223}},                // Pole 25
    {1, {230}},                // Pole 26
    {0, {}},                   // Pole 27
    {2, {209, 210}},           // Pole 28
    {1, {208}},                // Pole 29
    {1, {207}},                // Pole 30
    {1, {206}},                // Pole 31
    {1, {205}},                // Pole 32
    {0, {}},                   // Pole 33
    {0, {}},                   // Pole 34
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
    {0, {}},                   // Pole 47
    {2, {59, 60}},             // Pole 48
    {0, {}},                   // Pole 49
    {2, {61, 62}},             // Pole 50
    {0, {}},                   // Pole 51
    {0, {}},                   // Pole 52
    {0, {}},                   // Pole 53
    {1, {66}},                 // Pole 54
    {0, {}},                   // Pole 55
    {0, {}},                   // Pole 56
    {0, {}},                   // Pole 57
    {1, {186}},                // Pole 58
    {2, {183, 184}},           // Pole 59
    {1, {182}},                // Pole 60
    {1, {181}},                // Pole 61
    {1, {180}},                // Pole 62
    {0, {}},                   // Pole 63
    {0, {}},                   // Pole 64
    {1, {221}},                // Pole 65
    {1, {228}},                // Pole 66
    {0, {}},                   // Pole 67
    {2, {26, 27}},             // Pole 68
    {1, {25}},                 // Pole 69
    {1, {24}},                 // Pole 70
    {1, {23}},                 // Pole 71
    {0, {}},                   // Pole 72
    {1, {149}},                // Pole 73
    {1, {149}},                // Pole 74
    {1, {148}},                // Pole 75
    {1, {147}},                // Pole 76
    {1, {146}},                // Pole 77
    {1, {145}},                // Pole 78
    {1, {144}},                // Pole 79
    {0, {}},                   // Pole 80
    {1, {155}},                // Pole 81
    {1, {156}},                // Pole 82
    {1, {157}},                // Pole 83
    {2, {158, 159}},           // Pole 84
    {0, {}},                   // Pole 85
    {1, {137}},                // Pole 86
    {1, {136}},                // Pole 87
    {1, {135}},                // Pole 88
    {1, {135}},                // Pole 89
    {1, {134}},                // Pole 90
    {1, {134}},                // Pole 91
    {1, {132}},                // Pole 92
    {0, {}},                   // Pole 93
    {2, {163, 164}},           // Pole 94
    {2, {165, 166}},           // Pole 95
    {0, {}},                   // Pole 96
    {0, {}},                   // Pole 97
    {0, {}},                   // Pole 98
    {2, {114, 115}},           // Pole 99
    {1, {113}},                // Pole 100
    {1, {112}},                // Pole 101
    {1, {77}},                 // Pole 102
    {0, {}},                   // Pole 103
    {1, {80}},                 // Pole 104
    {2, {81, 82}},             // Pole 105
    {0, {}},                   // Pole 106
    {1, {84}},                 // Pole 107
    {1, {85}},                 // Pole 108
    {1, {86}},                 // Pole 109
    {1, {87}},                 // Pole 110
    {0, {}},                   // Pole 111
    {1, {90}},                 // Pole 112
    {2, {91, 92}},             // Pole 113
    {1, {93}},                 // Pole 114
    {3, {104, 105, 106}},      // Pole 115
    {1, {103}},                // Pole 116
    {1, {102}},                // Pole 117
    {0, {}},                   // Pole 118
    {1, {123}},                // Pole 119
    {1, {124}},                // Pole 120
    {1, {125}},                // Pole 121
    {1, {127}},                // Pole 122
    {1, {128}},                // Pole 123
    {0, {}},                   // Pole 124
    {0, {}},                   // Pole 125
    {0, {}},                   // Pole 126
    {1, {140}},                // Pole 127
    {1, {141}},                // Pole 128
    {2, {153, 154}},           // Pole 129
    {1, {152}},                // Pole 130
    {0, {}},                   // Pole 131
    {1, {21}},                 // Pole 132
    {1, {20}},                 // Pole 133
    {1, {19}},                 // Pole 134
    {1, {16}},                 // Pole 135
    {1, {15}},                 // Pole 136
    {0, {}},                   // Pole 137
    {1, {31}},                 // Pole 138
    {2, {32, 33}},             // Pole 139
    {2, {232, 233}},           // Pole 140
    {1, {212}},                // Pole 141
    {1, {214}},                // Pole 142
    {1, {215}},                // Pole 143
    {1, {216}},                // Pole 144
    {0, {}},                   // Pole 145
    {1, {192}},                // Pole 146
    {0, {}},                   // Pole 147
}
// Typ wyliczeniowy (Enum)
enum Zone {
    STREFA_A, STREFA_B, STREFA_C, STREFA_D, STREFA_E, STREFA_F,
    STREFA_G, STREFA_H, STREFA_I, STREFA_J, STREFA_K, STREFA_L
};

// Struktura opisująca pojedynczą strefę
struct ZoneContainer {
    const uint8_t* ledList;
    const uint8_t count;
};

// Tablica zbiorcza stref
const ZoneContainer zones[] = {
    {ledsA, sizeof(ledsA)},
    {ledsB, sizeof(ledsB)},
    {ledsC, sizeof(ledsC)},
    {ledsD, sizeof(ledsD)},
    {ledsE, sizeof(ledsE)},
    {ledsF, sizeof(ledsF)},
    {ledsG, sizeof(ledsG)},
    {ledsH, sizeof(ledsH)},
    {ledsI, sizeof(ledsI)},
    {ledsJ, sizeof(ledsJ)},
    {ledsK, sizeof(ledsK)},
    {ledsL, sizeof(ledsL)}
};

// --- Główna funkcja kolorująca wybraną strefę ---
void colorZone(Zone zone, CRGB color) {
    ZoneContainer currentZone = zones[zone];
    
    for (uint8_t i = 0; i < currentZone.count; i++) {
        int ledIndex = currentZone.ledList[i] - 1; // Konwersja 1-234 na indeksy 0-233
        
        if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
            leds[ledIndex] = color;
        }
    }
}

void setup() {
// Inicjalizacja paska LED
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(190); // Jasność ustawiona na 50%

    // Otwarcie kanału komunikacji z komputerem przez USB (baudrate: 9600)
    Serial.begin(9600);

    fill_solid(leds, NUM_LEDS, CRGB::Green); // fail-safe color
    // Wysłanie gotowych stanów do kontrolera paska
    FastLED.show();
}

void loop() {
    // 1. Sprawdzamy, czy przyszły jakieś dane
    if (Serial.available() > 0) {
        
        // 2. Czytamy całą paczkę danych (aż do znaku nowej linii \n)
        String incomingData = Serial.readStringUntil('\n');

        // 3. Upewniamy się, że paczka ma dokładnie 203 znaki (12 stref + 191 pól)
        if (incomingData.length() == 203) {
            
            // --- WARSTWA 1: STREFY (TŁO) ---
            for (int i = 0; i < 12; i++) {
                int trafficState = incomingData.charAt(i) - '0'; 
                
                CRGB newColor = CRGB::Green; // Domyślnie 0
                if (trafficState == 1) newColor = CRGB::Yellow;
                if (trafficState == 2) newColor = CRGB::Red;
                
                colorZone((Zone)i, newColor);
            }

            // --- WARSTWA 2: POLA (NAKŁADKA) ---
            for (int i = 0; i < 191; i++) {
                int fieldState = incomingData.charAt(12 + i) - '0';

                CRGB fieldColor = CRGB::Black;
                bool shouldColor = false; // Zmienna decydująca, czy zmieniamy kolor pola

                // Ustawiamy odpowiedni kolor dla graczy/pasażerów
                if (fieldState == 3) {
                    fieldColor = CRGB::Purple;
                    shouldColor = true;
                } 
                else if (fieldState == 4) {
                    fieldColor = CRGB::Blue;
                    shouldColor = true;
                } 
                else if (fieldState == 5) {
                    fieldColor = CRGB::White;
                    shouldColor = true;
                }
                
                // >>> TUTAJ DZIEJE SIĘ TWOJE "SKIP GDY ZERO" <<<
                // Jeśli shouldColor jest 'true' (stan 3, 4 lub 5), to zmieniamy kolory diod.
                // Jeśli stan to 0 (lub 1, lub 2), shouldColor wynosi 'false' i pętla omija ten fragment.
                // Dzięki temu na diodach pozostaje kolor strefy namalowany wyżej!
                if (shouldColor) {
                    for (int j = 0; j < boardFields[i].count; j++) {
                        int ledIndex = boardFields[i].leds[j] - 1; // Konwersja na indeks od 0
                        
                        if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
                            leds[ledIndex] = fieldColor; // Nadpisanie koloru
                        }
                    }
                }
            }
            
            // 4. Na koniec jednorazowo wysyłamy wszystko na pasek LED
            FastLED.show();
        }
    }
}