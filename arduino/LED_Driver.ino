#include <FastLED.h>

#define LED_PIN     5          // Zmień na numer pinu, do którego podpięty jest pasek
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

// --- Stan migających punktów spawnu (białe) ---
#define MAX_SPAWNS     32          // max. liczba migających LED-ów
#define BLINK_INTERVAL 400         // ms - co ile przełączamy biały <-> tło
#define BARRIER_COLOR  CRGB::Blue  // stały kolor barier (stan 5)

uint8_t       spawnLeds[MAX_SPAWNS];   // indeksy LED (0-based) do migania
CRGB          spawnUnder[MAX_SPAWNS];  // kolor strefy pod spotem (faza "off")
uint8_t       spawnCount = 0;
bool          blinkOn    = true;
unsigned long lastBlink  = 0;

// Nakłada bieżącą fazę migania na pasek (biały albo kolor tła pod spotem).
void renderSpawns() {
    for (uint8_t k = 0; k < spawnCount; k++) {
        leds[spawnLeds[k]] = blinkOn ? CRGB::White : spawnUnder[k];
    }
    FastLED.show();
}

void setup() {
// Inicjalizacja paska LED
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(127); // Jasność ustawiona na 50%

    // Otwarcie kanału komunikacji z komputerem przez USB (baudrate: 9600)
    Serial.begin(9600);

    fill_solid(leds, NUM_LEDS, CRGB::Green); // fail-safe color
    // Wysłanie gotowych stanów do kontrolera paska
    FastLED.show();
}

void loop() {
    unsigned long now = millis();

    // --- 1. Odbiór nowej ramki (jeśli jest) ---
    if (Serial.available() > 0) {
        // Cała ramka: 12 cyfr stref + opcjonalne indeksy LED po przecinku,
        // zakończona '\n'. Np.: "001020010200,8,61,62"
        static char frame[256];
        int len = Serial.readBytesUntil('\n', frame, sizeof(frame) - 1);

        if (len >= 12) { // potrzebujemy przynajmniej 12 cyfr stref
            // --- WARSTWA 1: STREFY (TŁO) ---
            for (int i = 0; i < 12; i++) {
                int trafficState = frame[i] - '0';
                CRGB newColor = CRGB::Green;            // 0 = zielony
                if (trafficState == 1) newColor = CRGB::Yellow;
                else if (trafficState == 2) newColor = CRGB::Red;
                colorZone((Zone)i, newColor);
            }

            // Separator ';' oddziela spoty (spawny) od barier:
            //   "<strefy>,<spawny...>;<bariery...>"
            int sep = len;
            for (int i = 12; i < len; i++) {
                if (frame[i] == ';') { sep = i; break; }
            }

            // --- WARSTWA 2: PUNKTY SPAWNU (biały, migają) ---
            // Indeksy LED w [12, sep). Zapamiętujemy je i kolor tła pod nimi.
            spawnCount = 0;
            for (int i = 12; i < sep; ) {
                if (frame[i] < '0' || frame[i] > '9') { i++; continue; }
                int ledNum = 0;
                while (i < sep && frame[i] >= '0' && frame[i] <= '9') {
                    ledNum = ledNum * 10 + (frame[i] - '0');
                    i++;
                }
                int ledIndex = ledNum - 1; // 1-234 -> 0-233
                if (ledIndex >= 0 && ledIndex < NUM_LEDS && spawnCount < MAX_SPAWNS) {
                    spawnLeds[spawnCount]  = ledIndex;
                    spawnUnder[spawnCount] = leds[ledIndex]; // kolor strefy pod spotem
                    spawnCount++;
                }
            }

            // --- WARSTWA 3: BARIERY (stały kolor, nie migają) ---
            // Indeksy LED po ';'. Znikają same, gdy program przestanie je wysyłać.
            for (int i = sep + 1; i < len; ) {
                if (frame[i] < '0' || frame[i] > '9') { i++; continue; }
                int ledNum = 0;
                while (i < len && frame[i] >= '0' && frame[i] <= '9') {
                    ledNum = ledNum * 10 + (frame[i] - '0');
                    i++;
                }
                int ledIndex = ledNum - 1;
                if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
                    leds[ledIndex] = BARRIER_COLOR;
                }
            }

            renderSpawns(); // od razu pokaż nową klatkę
        }
    }

    // --- 2. Miganie spotów na biało ---
    if (now - lastBlink >= BLINK_INTERVAL) {
        lastBlink = now;
        blinkOn = !blinkOn;
        renderSpawns();
    }
}