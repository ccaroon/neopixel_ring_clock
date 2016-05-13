#include <TimeLib.h>
#include "BinaryClock.h"

BinaryClock::BinaryClock(Adafruit_NeoPixel& neoPixelStrip) {
    strip = neoPixelStrip;

    strip.begin();

    color_h  = strip.Color(55,5,15);
    color_m  = strip.Color(31,52,0);
    color_s  = strip.Color(0,0,64);
}

void BinaryClock::initTime(const char* timeStr) {
    uint16_t hours, minutes, seconds;
    sscanf(timeStr, "%02d:%02d:%02d\n", &hours, &minutes, &seconds);

    setTime(hours, minutes, seconds, 1, 1, 2016);

    displayNumber(hour() > 12 ? hour()-12 : hour(), 4, HOURS_OFFSET,   color_h);
    displayNumber(minute(), 6, MINUTES_OFFSET, color_m);
    displayNumber(second(), 6, SECONDS_OFFSET, color_s);

    Serial.begin(9600);
    Serial.print("Hour: "); Serial.println(hours);

    lastUpdate = millis();
}

void BinaryClock::displayNumber(uint8_t value, uint8_t precision, uint8_t offset, uint32_t color) {
    for (uint8_t i = 0; i < precision; i++) {
        if (bitRead(value, i)) {
            strip.setPixelColor(i+offset, color);
        } else {
            strip.setPixelColor(i+offset, 0);
        }
    }
    strip.show();
}

void BinaryClock::tick() {
    if (millis() - lastUpdate >= 1000) {
        displayNumber(hour() > 12 ? hour()-12 : hour(), 4, HOURS_OFFSET,   color_h);
        displayNumber(minute(), 6, MINUTES_OFFSET, color_m);
        displayNumber(second(), 6, SECONDS_OFFSET, color_s);

        lastUpdate = millis();
    }
}
