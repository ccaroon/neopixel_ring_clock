#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEOPIN 6
#define LED_COUNT 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEOPIN, NEO_GRB + NEO_KHZ800);

#define SEC_SPEED 3750
#define MIN_SPEED 225000

uint32_t color_mh = strip.Color(16,16,16);
uint32_t color_s  = strip.Color(0,16,16);
uint32_t color_m  = strip.Color(16,0,16);
uint32_t color_h  = strip.Color(0,16,0);
                         // {12   1   2   3   4  5  6  7  8  9  10 11}
uint8_t hourPositions[12] = { 0, 15, 13, 12, 11, 9, 8, 7, 5, 4,  3, 1};

uint16_t hour, min, sec;
int hourPos;
int minPos;
int secPos;
unsigned long lastMinUpdate;
unsigned long lastSecUpdate;
uint32_t resetSecColor;
uint32_t resetMinColor;
// uint32_t resetHourColor;

void initTime(const char* timeStr) {
    sscanf(timeStr, "%02d:%02d:%02d\n", &hour, &min, &sec);
    Serial.println(timeStr);

    initHours();
    initMinutes();
    initSeconds();

    strip.show();

    lastMinUpdate  = lastSecUpdate  = millis();
}
void initHours() {
    if (hour >= 12) {
        hour -= 12;
    }
    hourPos = hourPositions[hour];
    Serial.println(hourPos);
    // resetHourColor = strip.getPixelColor(hourPos);
    strip.setPixelColor(hourPos, color_h);
}

void initMinutes() {
    minPos =  LED_COUNT - (min/3.75);
    Serial.println(minPos);
    resetMinColor = strip.getPixelColor(minPos);
    strip.setPixelColor(minPos, color_m);
}

void initSeconds() {
    secPos =  LED_COUNT - ((sec*1000)/3750);
    Serial.println(secPos);
    resetSecColor = strip.getPixelColor(secPos);
    strip.setPixelColor(secPos, color_s);
}

void setup() {
    strip.begin();
    Serial.begin(9600);

    // initTime(__TIME__);
    initTime("16:56:59");
}

// Seconds "hand" moves every 3,750ms
void advanceSeconds() {
    if (millis() - lastSecUpdate >= SEC_SPEED) {
        int oldPos = secPos;

        secPos--;
        if (secPos < 0) {
            secPos = strip.numPixels() - 1;
        }

        strip.setPixelColor(oldPos, resetSecColor);

        resetSecColor = strip.getPixelColor(secPos);
        if (resetSecColor == 0) {
            strip.setPixelColor(secPos, color_s);
        }
        strip.show();

        lastSecUpdate = millis();
    }
}

// Minute "hand" moves every 3.75 minutes = 3.75 * 60 = 225s = 225,000ms
void advanceMinutes() {
    if (millis() - lastMinUpdate >= MIN_SPEED) {
        int oldPos = minPos;

        minPos--;
        if (minPos < 0) {
            minPos = strip.numPixels() - 1;
        }

        strip.setPixelColor(oldPos, resetMinColor);

        resetMinColor = strip.getPixelColor(minPos);
        if (resetMinColor == color_h) {
            strip.setPixelColor(minPos, color_mh);
        } else {
            resetMinColor = 0; //black
            strip.setPixelColor(minPos, color_m);
        }

        strip.show();

        lastMinUpdate = millis();
    }
}

void advanceHours() {
    hour++;
    if (hour >= 12) {
        hour = 0;
    }
    hourPos = hourPositions[hour];
    strip.setPixelColor(hourPos, color_h);

    strip.show();
}

void loop() {
    advanceSeconds();
    advanceMinutes();
}
