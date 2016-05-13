#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "AnalogClock.h"
#include "BinaryClock.h"

#define NEOPIN 6
#define LED_COUNT 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEOPIN, NEO_GRB + NEO_KHZ800);
// AnalogClock analogClock(strip);
BinaryClock binaryClock(strip);

void setup() {
    Serial.begin(9600);
    // analogClock.initTime(__TIME__);
    binaryClock.initTime(__TIME__);
}

void loop() {
    // analogClock.tick();
    binaryClock.tick();
}
