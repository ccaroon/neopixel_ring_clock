#include "AnalogClock.h"

AnalogClock::AnalogClock(Adafruit_NeoPixel& neoPixelStrip) {
    strip = neoPixelStrip;

    strip.begin();

    color_hm = strip.Color(16,16,16);
    color_s  = strip.Color(0,16,16);
    color_m  = strip.Color(16,0,16);
    color_h  = strip.Color(0,16,0);

    ledCount = strip.numPixels();
}

void AnalogClock::initTime(const char* timeStr) {
    sscanf(timeStr, "%02d:%02d:%02d\n", &hour, &min, &sec);

    // Order matters
    initSeconds();
    initMinutes();
    initHours();

    strip.show();

    lastMinUpdate  = lastSecUpdate  = millis();
}

void AnalogClock::initHours() {
    if (hour >= 12) {
        hour -= 12;
    }
    hourPos = hourPositions[hour];

    uint32_t currColor = strip.getPixelColor(hourPos);
    if (currColor == color_m) {
        strip.setPixelColor(hourPos, color_hm);
    } else {
        strip.setPixelColor(hourPos, color_h);
    }
}

void AnalogClock::initMinutes() {
    minPos =  ledCount - (min/3.75);
    strip.setPixelColor(minPos, color_m);
}

void AnalogClock::initSeconds() {
    secPos =  ledCount - ((sec*1000)/3750);
    strip.setPixelColor(secPos, color_s);
}

// Seconds "hand" moves every 3,750ms
void AnalogClock::advanceSeconds() {
    if (millis() - lastSecUpdate >= SEC_SPEED) {
        int oldPos = secPos;

        secPos--;
        if (secPos < 0) {
            secPos = strip.numPixels() - 1;
        }

        uint32_t oldColor = strip.getPixelColor(oldPos);
        if (oldColor == color_s) {
            strip.setPixelColor(oldPos, 0);
        }

        uint32_t newColor = strip.getPixelColor(secPos);
        if (newColor == 0) {
            strip.setPixelColor(secPos, color_s);
        }
        strip.show();

        lastSecUpdate = millis();
    }
}

// Minute "hand" moves every 3.75 minutes = 3.75 * 60 = 225s = 225,000ms
void AnalogClock::advanceMinutes() {
    if (millis() - lastMinUpdate >= MIN_SPEED) {
        int oldPos = minPos;

        minPos--;
        if (minPos < 0) {
            minPos = strip.numPixels() - 1;
        }

        uint32_t oldColor = strip.getPixelColor(oldPos);
        if (oldColor == color_hm) {
            strip.setPixelColor(oldPos, color_h);
        } else {
            strip.setPixelColor(oldPos, 0);
        }

        uint32_t newColor = strip.getPixelColor(minPos);
        if (newColor == color_h) {
            strip.setPixelColor(minPos, color_hm);
        } else {
            strip.setPixelColor(minPos, color_m);
        }

        if (minPos == 0) {
            advanceHours();
        } else {
            strip.show();
        }

        lastMinUpdate = millis();
    }
}

void AnalogClock::advanceHours() {
    int oldPos = hourPos;

    hour++;
    if (hour >= 12) {
        hour = 0;
    }

    // What color should old hourPos be set to?
    uint32_t oldColor = strip.getPixelColor(oldPos);
    if (oldColor == color_hm) {
        strip.setPixelColor(oldPos, color_m);
    } else {
        strip.setPixelColor(oldPos, 0);
    }

    hourPos = hourPositions[hour];
    uint32_t newColor = strip.getPixelColor(hourPos);
    if (newColor == color_m) {
       strip.setPixelColor(hourPos, color_hm);
   } else {
       strip.setPixelColor(hourPos, color_h);
   }

    strip.show();
}

void AnalogClock::tick() {
    advanceSeconds();
    advanceMinutes();
}

char* AnalogClock::getState() {
    char stateStr[256];

    sprintf(stateStr, "h[%d] m[%d] s[%d]", hourPos, minPos, secPos);

    return stateStr;
}
