#include "Analog.h"

Analog::Analog(Adafruit_NeoPixel& neoPixelStrip) {
    strip = neoPixelStrip;

    strip.begin();

    color_mh = strip.Color(16,16,16);
    color_s  = strip.Color(0,16,16);
    color_m  = strip.Color(16,0,16);
    color_h  = strip.Color(0,16,0);

    ledCount = strip.numPixels();
}

void Analog::setTime(const char* timeStr) {
    sscanf(timeStr, "%02d:%02d:%02d\n", &hour, &min, &sec);
    Serial.println(timeStr);

    initHours();
    initMinutes();
    initSeconds();

    strip.show();

    lastMinUpdate  = lastSecUpdate  = millis();
}
void Analog::initHours() {
    if (hour >= 12) {
        hour -= 12;
    }
    hourPos = hourPositions[hour];
    Serial.println(hourPos);
    strip.setPixelColor(hourPos, color_h);
}

void Analog::initMinutes() {
    minPos =  ledCount - (min/3.75);
    Serial.println(minPos);
    resetMinColor = strip.getPixelColor(minPos);
    strip.setPixelColor(minPos, color_m);
}

void Analog::initSeconds() {
    secPos =  ledCount - ((sec*1000)/3750);
    Serial.println(secPos);
    resetSecColor = strip.getPixelColor(secPos);
    strip.setPixelColor(secPos, color_s);
}

// Seconds "hand" moves every 3,750ms
void Analog::advanceSeconds() {
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
void Analog::advanceMinutes() {
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

        if (minPos == 0) {
            advanceHours();
        } else {
            strip.show();
        }

        lastMinUpdate = millis();
    }
}

void Analog::advanceHours() {
    hour++;
    if (hour >= 12) {
        hour = 0;
    }

    uint32_t curColor = strip.getPixelColor(hourPos);
    if (curColor == color_mh) {
        strip.setPixelColor(hourPos, color_m);
    } else {
        strip.setPixelColor(hourPos, 0);
    }

    hourPos = hourPositions[hour];
    strip.setPixelColor(hourPos, color_h);

    strip.show();
}

void Analog::tick() {
    advanceSeconds();
    advanceMinutes();
}
