#ifndef Temperature_h
#define Temperature_h

#include <Arduino.h>
#include <EEPROM.h>

#include "../config/Hardware.h"
#include "../sensors/My_AHT10.h"

#define EEPROM_TEMP_OFFSET     40

class Temperature
{
  private:
    MyAHT10 *_sensor;
    float _currentTemperature;
    float _currentHumidity;
    float _currentDewpoint;
    float _maxTemperature;
    float _minTemperature;
    float _maxHumidity;
    float _minHumidity;    

    void WaitForInit();

  public: 
    Temperature();
    ~Temperature();

    float GetHumidity();
    float GetTemperature();
    float GetDewpoint();

    float GetMaxTemperature();
    float GetMinTemperature();
    float GetMaxHumidity();
    float GetMinHumidity();  
    void ResetMaxMin();
};

#endif