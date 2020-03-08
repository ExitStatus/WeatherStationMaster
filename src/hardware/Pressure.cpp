#include "Pressure.h"

Pressure::Pressure(int altitude)
{
    _altitude = altitude;
    _bmpSensor = new My_BMP280(&Wire);

    if (!_bmpSensor->begin(I2C_BMP280_ADDRESS)) {
        Serial.println(F("An error occurred initializing BMP280"));
        return;
    }

    _bmpSensor->setSampling(My_BMP280::MODE_NORMAL,     /* Operating Mode. */
        My_BMP280::SAMPLING_X1,     /* Temp. oversampling */
        My_BMP280::SAMPLING_X4,    /* Pressure oversampling */
        My_BMP280::FILTER_X16,      /* Filtering. */
        My_BMP280::STANDBY_MS_500); /* Standby time. */

    if (EEPROM.read(EEPROM_PRESSURE_OFFSET) != 0xbe && EEPROM.read(EEPROM_PRESSURE_OFFSET+1) != 0xef)
    {
        EEPROM.write(EEPROM_PRESSURE_OFFSET, 0xbe);
        EEPROM.write(EEPROM_PRESSURE_OFFSET+1, 0xef);

        float newValue = _bmpSensor->readPressure() / 100;
        if (!isnan(newValue)) 
        {
            newValue = _bmpSensor->seaLevelForAltitude(_altitude, newValue);

            EEPROM.put(EEPROM_PRESSURE_OFFSET + 2, newValue);
            EEPROM.put(EEPROM_PRESSURE_OFFSET + 2 + sizeof(float), newValue);

            _maxPressure = _minPressure = newValue;
        }

        EEPROM.commit();
    }
    else
    {
        EEPROM.get(EEPROM_PRESSURE_OFFSET + 2, _minPressure);
        EEPROM.get(EEPROM_PRESSURE_OFFSET + 2 + sizeof(float), _maxPressure);

        Serial.print(F("EEPROM: Pressure: "));
        Serial.print(_minPressure);
        Serial.print(" / ");
        Serial.println(_maxPressure);
    }

    GetPressure();
}

Pressure::~Pressure()
{
    delete _bmpSensor;
}

void Pressure::SetAltitude(int altitude)
{
    _altitude = altitude;
}

bool Pressure::GetPressure(float &pressureValue)
{
    float newValue = _bmpSensor->readPressure() / 100;

    if (isnan(newValue)) 
    {
        Serial.println(F("Failed to read from BMP280 sensor!"));
        pressureValue = _currentPressure;
        return false;
    }

    _currentPressure = _bmpSensor->seaLevelForAltitude(_altitude, newValue);

    if (_currentPressure < _minPressure)
    {
        _minPressure = _currentPressure;
        EEPROM.put(EEPROM_PRESSURE_OFFSET + 2, _minPressure);
        EEPROM.commit();
    }
 
    if (_currentPressure > _maxPressure)
    {
        _maxPressure = _currentPressure;
        EEPROM.put(EEPROM_PRESSURE_OFFSET + 2 + sizeof(float), _maxPressure);
        EEPROM.commit();
    }

    pressureValue = _currentPressure;

    return true;
}

float Pressure::GetPressure()
{
    float pressureValue;

    GetPressure(pressureValue);

    return pressureValue;
}

float Pressure::GetMaxPressure()
{
    return _maxPressure;
}

float Pressure::GetMinPressure()
{
    return _minPressure;
}

void Pressure::ResetMaxMin()
{
    _maxPressure = _minPressure = _currentPressure;

    EEPROM.put(EEPROM_PRESSURE_OFFSET + 2, _minPressure);
    EEPROM.put(EEPROM_PRESSURE_OFFSET + 2 + sizeof(float), _maxPressure);
    EEPROM.commit();
}