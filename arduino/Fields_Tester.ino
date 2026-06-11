#include <FastLED.h>

#define LED_PIN     6          // Zmień na numer pinu, do którego podpięty jest pasek
#define NUM_LEDS    234        // Całkowita liczba LED-ów na pasku
#define LED_TYPE    WS2812B    // Typ paska
#define COLOR_ORDER GRB        // Kolejność kolorów

CRGB leds[NUM_LEDS];

// --- Definicje stref ---
const uint8_t ledsA[] = {178,179, 180, 181,192, 193, 194, 209,210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,228, 229, 230, 231, 232, 233};
const uint8_t ledsB[] = {13, 14, 15, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 224,225, 226, 227};
const uint8_t ledsC[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
 uint8_t ledsD[] = {1, 2, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 147,148, 149, 150, 151, 152};
const uint8_t ledsE[] = {130,131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 153,154, 155, 156, 157, 158, 159, 160, 161, 162, 163};
const uint8_t ledsF[] = {125,126, 127, 128, 129, 164,165, 166, 171,172, 173, 174, 175, 176, 177};
const uint8_t ledsG[] = {99, 100, 101, 102, 103, 114,115, 116, 117, 118, 119, 120, 121, 122, 123, 124,167, 168, 169, 170};
const uint8_t ledsH[] = {72,73, 74, 75, 76, 77, 78, 80,91, 92, 93, 94, 95, 96, 97, 98, 104,105, 106, 107, 108, 109, 110, 111, 112, 113};
const uint8_t ledsI[] = {79,80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
const uint8_t ledsJ[] = {67, 68, 69, 70, 71,182, 183, 184, 185, 186, 187, 188, 189, 190, 191};
const uint8_t ledsK[] = {36, 37, 38, 39, 59,60, 61, 62, 63, 64, 65, 66, 195,196, 197, 206,207, 208};
const uint8_t ledsL[] = {40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,198, 199, 200, 201, 202, 203, 204, 205};

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
    FastLED.setBrightness(127); // Jasność ustawiona na 50%

    // Najpierw czyścimy pasek (tło np. wyłączone lub zielone)
    fill_solid(leds, NUM_LEDS, CRGB(0,0,0)); 

    for(int i=0; i < 234; i++){
      leds[i] = CRGB(255,255,255);
      delay(3000);
      leds[i] = CRGB(0,0,0);
    }

    // Wysłanie gotowych stanów do kontrolera paska
    FastLED.show();
}

void loop() {
    // Pętla pozostaje pusta na czas testów
}
