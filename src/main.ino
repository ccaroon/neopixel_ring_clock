#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "Analog.h"

#define NEOPIN 6
#define LED_COUNT 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEOPIN, NEO_GRB + NEO_KHZ800);
Analog analogClock(strip);

void setup() {
    Serial.begin(9600);
    analogClock.setTime("11:50:45");
}

void loop() {
    analogClock.tick();
    Serial.println(analogClock.getState());
    delay(1000);
}
