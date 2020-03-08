#include "Stats.h"

Stats::Stats(Tiny_SH1106 *lcd)
{
  _lcd = lcd;

  _temperatureSensor = new Temperature();
  _pressureSensor = new Pressure(98);

  _timer = new Interval();
  _timer->Start(5000, true);
}

Stats::~Stats()
{
  delete _timer;
  delete _pressureSensor;
  delete _temperatureSensor;
}

void Stats::SetMode(int mode)
{
  _mode = mode;
  _timer->Now();
}

void Stats::ResetMaxMin()
{
  _temperatureSensor->ResetMaxMin();
  _pressureSensor->ResetMaxMin();
}

void Stats::Render()
{
  if (!_timer->Ready())
      return;

  _lcd->setFont(NULL);
  _lcd->setTextSize(1);
  _lcd->setTextColor(WHITE);
  
  if (_mode > 1)
    _lcd->fillRect(0, 0, 128, 64, BLACK);   
  else
    _lcd->fillRect(0, 38, 128, 26, BLACK);   

  float temperature = _temperatureSensor->GetTemperature();
  float humidity = _temperatureSensor->GetHumidity();
  float dewpoint = _temperatureSensor->GetDewpoint();
  float pressure = _pressureSensor->GetPressure();

  switch (_mode)
  {
    case 0: RenderStyle0(temperature, humidity, dewpoint, pressure); break;
    case 1: RenderStyle1(temperature, humidity, dewpoint, pressure); break;
    case 2: RenderStyle2(temperature, humidity, dewpoint, pressure); break;
    case 3: RenderStyle3(temperature, humidity, dewpoint, pressure); break;
    case 4: RenderStyle4(temperature, humidity, dewpoint, pressure); break;
  }

  _lcd->display();
}

void Stats::printTemp(int x, int y, char *prefix, int temp)
{
  _lcd->setCursor(x,y);

  char *buffer = new char[6 + strlen(prefix)];
  sprintf(buffer, "%s%d\0", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F(" C"));

  _lcd->drawBitmap(x + (strlen(buffer) * 6), y,  degreeIcon, 8, 8, WHITE);

  delete[] buffer;
}

void Stats::printTemp(int x, int y, char *prefix, float temp)
{
  _lcd->setCursor(x,y);

  char *buffer = new char[9+strlen(prefix)];
  sprintf(buffer, "%s%.2f\0", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F(" C"));

  _lcd->drawBitmap(x + (strlen(buffer) * 6), y,  degreeIcon, 8, 8, WHITE);

  delete[] buffer;
}

void Stats::printHumidity(int x, int y, char *prefix, float temp)
{
  _lcd->setCursor(x,y);

  char *buffer = new char[9+strlen(prefix)];
  sprintf(buffer, "%s%.2f\0", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F("%"));
  delete[] buffer;
}
    
void Stats::printPressure(int x, int y, char *prefix, float temp)
{
  _lcd->setCursor(x,y);

  char *buffer = new char[9+strlen(prefix)];
  sprintf(buffer, "%s%.2f\0", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F("mb"));
  delete[] buffer;
}

void Stats::RenderStyle0(float temp, float humid, float dew, float pressure)
{
  _lcd->drawBitmap(20, 38, thermometerIcon, 16, 16, 1);
  _lcd->drawBitmap(56, 38,  dripIcon, 16, 16, 1);
  _lcd->drawBitmap(92, 38,  pressureIcon, 16, 16, 1);

  char *buffer = new char[6];
  sprintf(buffer, "%d C\0", (int)round(temp));
    
  if ((int)round(temp) < 10)
  {
    int x = 27 - ((3 * 6)/2);
    _lcd->setCursor(x,57);
    _lcd->print(buffer);
    _lcd->drawBitmap(x+7, 57,  degreeIcon, 8, 8, WHITE);
  }
  else
  {
    int x = 27 - ((4 * 5)/2);
    _lcd->setCursor(x,57);
    _lcd->print(buffer);
    _lcd->drawBitmap(x+12,57,  degreeIcon, 8, 8, WHITE);
  }

  sprintf(buffer, "%d%%\0", (int)round(humid));
  _lcd->setCursor(64 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 

  sprintf(buffer, "%d\0", (int)round(pressure));
  _lcd->setCursor(100 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 

  delete[] buffer;
}

void Stats::RenderStyle1(float temp, float humid, float dew, float pressure)
{
  _lcd->drawFastHLine(0, 38, 128, WHITE);

  printTemp(0, 44, " Temp:", (int)round(temp));
  printTemp(66, 44, "DewP:", (int)round(dew));
  
  _lcd->setCursor(0,56);
  _lcd->print(F("Humid:"));
  _lcd->print((int)round(humid));
  _lcd->print(F("%"));

  _lcd->setCursor(66,56);
  _lcd->print(F("Mbar:"));
  _lcd->print((int)round(pressure));
}

void Stats::RenderStyle2(float temp, float humid, float dew, float pressure)
{
  _lcd->setCursor(28,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  char *buffer = new char[16];
  sprintf(buffer, "%0.2f", temp);
  _lcd->print(buffer);
  delete[] buffer;

  _lcd->setFont();
  _lcd->setCursor(64 - ((18 * 6)/2) ,25);
  _lcd->print("Degrees Centigrade");

  _lcd->drawFastHLine(0, 38, 128, WHITE);

  printTemp(0, 42, "Max:", _temperatureSensor->GetMaxTemperature());
  printTemp(0, 54, "Min:", _temperatureSensor->GetMinTemperature());
}

void Stats::RenderStyle3(float temp, float humid, float dew, float pressure)
{
  _lcd->setCursor(28,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  char *buffer = new char[16];
  sprintf(buffer, "%0.2f", humid);
  _lcd->print(buffer);
  delete[] buffer;

  _lcd->setFont();
  _lcd->setCursor(64 - ((16 * 6)/2) ,25);
  _lcd->print("Percent Humidity");

  _lcd->drawFastHLine(0, 38, 128, WHITE);

  printHumidity(0, 42, "Max:", _temperatureSensor->GetMaxHumidity());
  printHumidity(0, 54, "Min:", _temperatureSensor->GetMinHumidity());
}

void Stats::RenderStyle4(float temp, float humid, float dew, float pressure)
{
  _lcd->setCursor(10,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  char *buffer = new char[16];
  sprintf(buffer, "%0.2f%", pressure);
  _lcd->print(buffer);
  delete[] buffer;

  _lcd->setFont();
  _lcd->setCursor(64 - ((18 * 6)/2) ,25);
  _lcd->print("Millibars Pressure");

  _lcd->drawFastHLine(0, 38, 128, WHITE);

  printPressure(0, 42, "Max:", _pressureSensor->GetMaxPressure());
  printPressure(0, 54, "Min:", _pressureSensor->GetMinPressure());
}
