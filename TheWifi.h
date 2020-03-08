#ifndef _THEWIFI_H
#define _THEWIFI_H

#include "WiFi.h"
#include "time.h"

#include "src/sensors/Tiny_SH1106.h"
#include "src/hardware/Interval.h"
#include "src/bitmaps/WifiIcons.h"

#include "ClockTime.h"

class TheWifi
{
  private:
    int _networkId;
    bool _connected = false;
    Interval *_timer;
    Tiny_SH1106 *_lcd;
    
  public: 
    TheWifi(int networkId, Tiny_SH1106 *lcd);
    ~TheWifi();

    bool IsConnected();
    void Render();
    int GetStrength();
    const __FlashStringHelper *GetStatus();
    String GetSSID();
    int GetRSSI();
    IPAddress GetIP();
    uint32_t GetNtpTime();
    void SetNtpTime(ClockTime *clockTime);
}; 
#endif