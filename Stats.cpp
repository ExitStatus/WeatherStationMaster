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
    _lcd->fillRect(1, 0, 128, 64, BLACK);   
  else
    _lcd->fillRect(1, 38, 128, 26, BLACK);   

  float temperature = _temperatureSensor->GetTemperature();
  float humidity = _temperatureSensor->GetHumidity();
  float dewpoint = _temperatureSensor->GetDewpoint();
  float pressure = _pressureSensor->GetPressure();

  switch (_mode)
  {
    case 0: RenderStyle0(); break;
    case 1: RenderStyle1(); break;
    case 2: RenderStyle2(); break;
    case 3: RenderStyle3(); break;
    case 4: RenderStyle4(); break;
  }

  _lcd->display();
}

void Stats::printTemp(int x, int y, const __FlashStringHelper *prefix, int temp)
{
  char buffer[64];

  _lcd->setCursor(x,y);

  sprintf(buffer, "%s%d", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F(" C"));

  _lcd->drawBitmap(x + (strlen(buffer) * 6), y,  degreeIcon, 8, 8, WHITE);

}

void Stats::printTemp(int x, int y, const __FlashStringHelper *prefix, float temp)
{
  char buffer[64];

  _lcd->setCursor(x,y);

  sprintf(buffer, "%s%.2f", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F(" C"));

  _lcd->drawBitmap(x + (strlen(buffer) * 6), y,  degreeIcon, 8, 8, WHITE);
}

void Stats::printHumidity(int x, int y, const __FlashStringHelper *prefix, float temp)
{
  char buffer[64];

  _lcd->setCursor(x,y);

  sprintf(buffer, "%s%.2f", prefix, temp);

  _lcd->print(buffer);
  _lcd->print(F("%"));
}
    
void Stats::printPressure(int x, int y, const __FlashStringHelper *prefix, float pressure)
{
  char buffer[64];

  _lcd->setCursor(x,y);

  sprintf(buffer, "%s%.2f", prefix, pressure);

  _lcd->print(buffer);
  _lcd->print(F("mb"));
}

void Stats::RenderStyle0()
{
  char buffer[12];

  _lcd->drawBitmap(20, 38, thermometerIcon, 16, 16, 1);
  _lcd->drawBitmap(56, 38,  dripIcon, 16, 16, 1);
  _lcd->drawBitmap(92, 38,  pressureIcon, 16, 16, 1);

  float temp = _temperatureSensor->GetTemperature();

  sprintf(buffer, "%d C", (int)round(temp));
    
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

  sprintf(buffer, "%d%%", (int)round(_temperatureSensor->GetHumidity()));
  _lcd->setCursor(64 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 

  sprintf(buffer, "%d", (int)round(_pressureSensor->GetPressure()));
  _lcd->setCursor(100 - ((strlen(buffer) * 6) / 2), 57);
  _lcd->print(buffer); 
}

void Stats::RenderStyle1()
{
  _lcd->drawFastHLine(1, 38, 128, WHITE);

  printTemp(1, 44, F(" Temp:"), (int)round(_temperatureSensor->GetTemperature()));
  printTemp(67, 44, F("DewP:"), (int)round(_temperatureSensor->GetDewpoint()));
  
  _lcd->setCursor(1,56);
  _lcd->print(F("Humid:"));
  _lcd->print((int)round(_temperatureSensor->GetHumidity()));
  _lcd->print(F("%"));

  _lcd->setCursor(67,56);
  _lcd->print(F("Mbar:"));
  _lcd->print((int)round(_pressureSensor->GetPressure()));
}

void Stats::RenderStyle2()
{
  char buffer[16];

  _lcd->setCursor(28,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  sprintf(buffer, "%0.2f", _temperatureSensor->GetTemperature());
  _lcd->print(buffer);

  _lcd->setFont();
  _lcd->setCursor(64 - ((18 * 6)/2) ,25);
  _lcd->print(F("Degrees Centigrade"));

  _lcd->drawFastHLine(1, 38, 128, WHITE);

  printTemp(1, 42, F("Max: "), _temperatureSensor->GetMaxTemperature());
  printTemp(1, 54, F("Min: "), _temperatureSensor->GetMinTemperature());
}

void Stats::RenderStyle3()
{
  char buffer[16];

  _lcd->setCursor(28,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  sprintf(buffer, "%0.2f", _temperatureSensor->GetHumidity());
  _lcd->print(buffer);

  _lcd->setFont();
  _lcd->setCursor(64 - ((16 * 6)/2) ,25);
  _lcd->print(F("Percent Humidity"));

  _lcd->drawFastHLine(1, 38, 128, WHITE);

  printHumidity(1, 42, F("Max: "), _temperatureSensor->GetMaxHumidity());
  printHumidity(1, 54, F("Min: "), _temperatureSensor->GetMinHumidity());
}

void Stats::RenderStyle4()
{
  char buffer[16];

  _lcd->setCursor(10,14);
  _lcd->setTextColor(WHITE);
  _lcd->setFont(&DSEG7_Classic_Regular_20);

  sprintf(buffer, "%0.2f%", _pressureSensor->GetPressure());
  _lcd->print(buffer);

  _lcd->setFont();
  _lcd->setCursor(64 - ((18 * 6)/2) ,25);
  _lcd->print(F("Millibars Pressure"));

  _lcd->drawFastHLine(1, 38, 128, WHITE);

  printPressure(1,42,F("Max: "), _pressureSensor->GetMaxPressure());
  printPressure(1,54,F("Min: "), _pressureSensor->GetMinPressure());
}
