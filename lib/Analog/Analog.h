#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define SEC_SPEED 3750
#define MIN_SPEED 225000

class Analog {
public:
    Analog(Adafruit_NeoPixel&);

    void setTime(const char* timeStr);

    void tick();
    void advanceSeconds();
    void advanceMinutes();
    void advanceHours();

    char* getState();
private:
    Adafruit_NeoPixel strip;

    uint8_t ledCount;

    uint32_t color_hm;
    uint32_t color_s;
    uint32_t color_m;
    uint32_t color_h;
                             // {12   1   2   3   4  5  6  7  8  9  10 11}
    uint8_t hourPositions[12] = { 0, 15, 13, 12, 11, 9, 8, 7, 5, 4,  3, 1};

    uint16_t hour, min, sec;
    int hourPos;
    int minPos;
    int secPos;
    unsigned long lastMinUpdate;
    unsigned long lastSecUpdate;

    void initHours();
    void initMinutes();
    void initSeconds();
};
