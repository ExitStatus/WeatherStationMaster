#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "src/sensors/Tiny_SH1106.h"

#include "src/hardware/Interval.h"
#include "src/hardware/Button.h"

#include "Stats.h"
#include "TheWifi.h"

class Settings
{
    private:
        Tiny_SH1106 *_lcd;
        Button *_leftButton; 
        Button *_middleButton; 
        Button *_rightButton;
        Stats *_stats;
        TheWifi *_network;

        void DrawMenu(int firstItem, int selectedItem);
        void ChangeSetting(int selectedItem);
        void ResetMaxMin();

    public:
        Settings(Tiny_SH1106 *lcd, Button *leftButton, Button *middleButton, Button *rightButton, Stats *stats,  TheWifi *network);
        void UpdateSettingsUI();
        void MenuWifi();

};

#endif