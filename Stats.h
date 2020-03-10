#ifndef Stats_h
#define Stats_h

#include <Arduino.h>

#include "src/config/Hardware.h"
#include "src/sensors/My_AHT10.h"
#include "src/sensors/Tiny_SH1106.h"
#include "src/hardware/Interval.h"
#include "src/hardware/Pressure.h"
#include "src/hardware/Temperature.h"
#include "src/fonts/LCDFont.h"

#include "src/bitmaps/DegreeIcon.h"
#include "src/bitmaps/ThermometerIcon.h"
#include "src/bitmaps/DripIcon.h"
#include "src/bitmaps/PressureIcon.h"

class Stats
{
  private:
    Interval *_timer;
    Tiny_SH1106 *_lcd;

    Temperature *_temperatureSensor;
    Pressure *_pressureSensor;

    uint8_t _mode = 0;

    void RenderStyle0();
    void RenderStyle1();
    void RenderStyle2();
    void RenderStyle3();
    void RenderStyle4();

    void printTemp(int x, int y, const __FlashStringHelper *prefix, int temp);
    void printTemp(int x, int y, const __FlashStringHelper *prefix, float temp);
    void printHumidity(int x, int y, const __FlashStringHelper *prefix, float temp);
    void printPressure(int x, int y, const __FlashStringHelper *prefix, float pressure);
  public: 
    Stats(Tiny_SH1106 *lcd);
    ~Stats();
    void SetMode(int mode);
    void Render();
    void ResetMaxMin();
    void WaitForInit();
};

#endif
