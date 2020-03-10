                                                                                                                                                                                                                                                                                                                                                      #include <Wire.h>
//#//include "Tiny_SH1106.h"

#include <Wire.h>
#include <EEPROM.h>

#include "src/hardware/Button.h"
#include "src/config/Hardware.h"

#include "Stats.h"
#include "ClockTime.h"
#include "Settings.h"
#include "TheWifi.h"

#define EEPROM_SIZE  250

#define OLED_RESET 4
Tiny_SH1106 lcd(OLED_RESET);

Stats *stats;
ClockTime *clockTime;
Button *leftButton; 
Button *middleButton; 
Button *rightButton; 
TheWifi *network;

Interval *ntpInterval;

int mode = 0;

void setup() 
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // SDA, SCL
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(I2C_SPEED);

  lcd.begin(SH1106_SWITCHCAPVCC, I2C_OLED_ADDRESS);
  lcd.clearDisplay();
  lcd.setCursor(43,26);
  lcd.setTextSize(1);
  lcd.setTextColor(WHITE);
  lcd.print("Welcome");
  lcd.display();

  EEPROM_Init();

  stats = new Stats(&lcd);

  clockTime = new ClockTime(&lcd);
  leftButton = new Button(PIN_LEFT);
  middleButton = new Button(PIN_MIDDLE);
  rightButton = new Button(PIN_RIGHT);

  network = new TheWifi(0, &lcd);
  ntpInterval = new Interval(3600000, true);
}

void loop() 
{
  if (rightButton->State() == BUTTON_CLICKED)
  {
    Serial.println("Right button click");
    NextMode();
  }

  if (leftButton->State() == BUTTON_CLICKED)
  {
    Serial.println("Left button click");
    PrevMode();
  }

  if (middleButton->State() == BUTTON_HELD)
  {
    Serial.println("Middle button held");

    Settings *settings = new Settings(&lcd, leftButton, middleButton, rightButton, stats, network);
    settings->UpdateSettingsUI();
    delete settings;
    
    clockTime->Render(false);
  }
  else
    clockTime->Render(true);
  
  stats->Render();

  if (mode == 0)
    network->Render();

  lcd.display();

  if (ntpInterval->Ready())
  {
    network->SetNtpTime(clockTime);
  }
}

void NextMode()
{
  if (++mode > 4)
    mode = 0;

  clockTime->SetMode(mode);
  stats->SetMode(mode);
}

void PrevMode()
{
  if (mode == 0)
    mode = 4;
  else
    mode--;
  
  clockTime->SetMode(mode);
  stats->SetMode(mode);
}

void EEPROM_Init()
{
  EEPROM.begin(EEPROM_SIZE);

  Serial.print(F("EEPROM_Init ("));
  Serial.print(EEPROM_SIZE);
  Serial.println(F(" bytes storage size)"));
  
  bool reset = false;

  if (reset || EEPROM.read(0) != 0xbe && EEPROM.read(1) != 0xef)
  {
    for (int i = 0 ; i < 100 ; i++) 
        EEPROM.write(i, 0);
    
    EEPROM.write(0, 0xbe);
    EEPROM.write(1, 0xef);
    EEPROM.commit();

    Serial.println(F("EEPROM BOM Written"));
  }
  else
    Serial.println(F("EEPROM BOM Found"));
}