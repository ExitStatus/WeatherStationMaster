#ifndef Pressure_h
#define Pressure_h

#include <Arduino.h>
#include <EEPROM.h>

#include "../config/Hardware.h"
#include "../sensors/My_BMP280.h"

#define EEPROM_PRESSURE_OFFSET     20

class Pressure
{
  private:
    My_BMP280 *_bmpSensor;
    float _currentPressure;
    float _maxPressure;
    float _minPressure;
    int _altitude = 0;
    
  public: 
    Pressure(int altitude);
    ~Pressure();

    void SetAltitude(int altitude);
    bool GetPressure(float &pressureValue);
    float GetPressure();
    float GetMaxPressure();
    float GetMinPressure();
    void ResetMaxMin();
};

#endif