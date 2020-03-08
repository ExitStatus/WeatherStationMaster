#include "Temperature.h"

Temperature::Temperature()
{
    _sensor = new MyAHT10();

    if (!_sensor->begin(I2C_AHT10_ADDRESS))
    {
        Serial.println(F("An error occurred initializing AHT10"));
        return;
    }

    WaitForInit();

    if (EEPROM.read(EEPROM_TEMP_OFFSET) != 0xbe && EEPROM.read(EEPROM_TEMP_OFFSET+1) != 0xef)
    {
        EEPROM.write(EEPROM_TEMP_OFFSET, 0xbe);
        EEPROM.write(EEPROM_TEMP_OFFSET+1, 0xef);

        float newValue = _sensor->GetTemperature();
        if (!isnan(newValue)) 
        {
            EEPROM.put(EEPROM_TEMP_OFFSET + 2, newValue);
            EEPROM.put(EEPROM_TEMP_OFFSET + 2 + sizeof(float), newValue);

            _maxTemperature = _minTemperature = newValue;
        }

        newValue = _sensor->GetHumidity();
        if (!isnan(newValue)) 
        {
            EEPROM.put(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 2), newValue);
            EEPROM.put(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 3), newValue);

            _maxHumidity = _minHumidity = newValue;
        }

        EEPROM.commit();
    }
    else
    {
        EEPROM.get(EEPROM_TEMP_OFFSET + 2, _minTemperature);
        EEPROM.get(EEPROM_TEMP_OFFSET + 2 + sizeof(float), _maxTemperature);
        EEPROM.get(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 2), _minHumidity);
        EEPROM.get(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 3), _maxHumidity);

        Serial.print(F("EEPROM: Temperature: "));
        Serial.print(_minTemperature);
        Serial.print(" / ");
        Serial.println(_maxTemperature);

        Serial.print(F("EEPROM: Humidity: "));
        Serial.print(_minHumidity);
        Serial.print(" / ");
        Serial.println(_maxHumidity);
    }
}

Temperature::~Temperature()
{
    delete _sensor;
}

void Temperature::WaitForInit()
{
    int lastVal = _sensor->GetTemperature();
    int count = 1;

    while (true)
    {
        int nextVal = _sensor->GetTemperature();
        if (nextVal > -20)
        {
            if (nextVal == lastVal)
            {
                count++;
                if (count > 3)
                    return;
            }
            else
            {
                lastVal = nextVal;
                count = 0;
            }
        }

        delay(250);
    }
}

float Temperature::GetTemperature()
{
    float newValue = _sensor->GetTemperature();
    if (isnan(newValue)) 
    {
        Serial.println(F("Failed to read temperature from AHT10 sensor!"));
        return _currentTemperature;
    }

    _currentTemperature = newValue;

    if (_currentTemperature < _minTemperature)
    {
        _minTemperature = _currentTemperature;
        EEPROM.put(EEPROM_TEMP_OFFSET + 2, _minTemperature);
        EEPROM.commit();
    }
 
    if (_currentTemperature > _maxTemperature)
    {
        _maxTemperature = _currentTemperature;
        EEPROM.put(EEPROM_TEMP_OFFSET + 2 + sizeof(float), _maxTemperature);
        EEPROM.commit();
    }

    return _currentTemperature;
}

float Temperature::GetHumidity()
{
    float newValue = _sensor->GetHumidity();
    if (isnan(newValue)) 
    {
        Serial.println(F("Failed to read humidity from AHT10 sensor!"));
        return _currentHumidity;
    }

    _currentHumidity = newValue;

    if (_currentHumidity < _minHumidity)
    {
        _minHumidity = _currentHumidity;
        EEPROM.put(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 2), _minHumidity);
        EEPROM.commit();
    }
 
    if (_currentHumidity > _maxHumidity)
    {
        _maxHumidity = _currentHumidity;
        EEPROM.put(EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 3), _maxHumidity);
        EEPROM.commit();
    }

    return _currentHumidity;
}

float Temperature::GetDewpoint()
{
    float newValue = _sensor->GetDewPoint();
    if (isnan(newValue)) 
    {
        Serial.println(F("Failed to read dew point from AHT10 sensor!"));
        return _currentDewpoint;
    }

    _currentDewpoint = newValue;

    return _currentDewpoint;
}

float Temperature::GetMaxTemperature()
{
    return _maxTemperature;
}

float Temperature::GetMinTemperature()
{
    return _minTemperature;
}

float Temperature::GetMaxHumidity()
{
    return _maxHumidity;
}

float Temperature::GetMinHumidity()
{
    return _minHumidity;
}

void Temperature::ResetMaxMin()
{
    _minTemperature = _maxTemperature = _currentTemperature;
    _maxHumidity = _minHumidity = _currentHumidity;

    WriteToEEPROM(F("min temperature"), _minTemperature, EEPROM_TEMP_OFFSET + 2);
    WriteToEEPROM(F("max temperature"), _maxTemperature, EEPROM_TEMP_OFFSET + 2 + sizeof(float));
    WriteToEEPROM(F("min humidity"), _minHumidity, EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 2));
    WriteToEEPROM(F("max humidity"), _maxHumidity, EEPROM_TEMP_OFFSET + 2 + (sizeof(float) * 3));

    EEPROM.commit();
}

void Temperature::WriteToEEPROM(const __FlashStringHelper *description, float value, int offset)
{
    Serial.print(F("Writing "));
    Serial.print(description);
    Serial.print(F(" = "));
    Serial.print(value);
    Serial.print(F(" to EEPROM at offset "));
    Serial.println(offset);

    EEPROM.put(offset, value);
}