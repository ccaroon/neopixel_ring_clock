#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define HOURS_OFFSET   0
#define MINUTES_OFFSET 4
#define SECONDS_OFFSET 10

class BinaryClock {
public:
    BinaryClock(Adafruit_NeoPixel&);

    void initTime(const char* timeStr);

    void tick();
private:
    Adafruit_NeoPixel strip;

    uint32_t color_h, color_m, color_s;
    unsigned long lastUpdate;

    void displayNumber(uint8_t,uint8_t,uint8_t,uint32_t);
};
