#ifndef ClockTime_h
#define ClockTime_h

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <string.h>
#include <RtcDS3231.h>
#include <WiFi.h>

#include "src/sensors/Tiny_SH1106.h"
#include "src/fonts/LCDFont.h"
#include "src/hardware/Interval.h"
#include "src/config/Hardware.h"



class ClockTime
{
  private:
    Interval *_timer;
    Tiny_SH1106 *_lcd;
    RtcDS3231<TwoWire> *_rtc;
    bool _clockDots = false;
    int _mode = 0;

    void CheckNTP();

  public: 
    ClockTime(Tiny_SH1106 *lcd);
    ~ClockTime();

    void Render(bool display);
    void SetMode(int mode);

    void GetIso8601(char* buffer);
    void SetTime(struct tm newTime);
};

#endif
