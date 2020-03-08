#include "TheWifi.h"

char *ssids[1] = { "SKYDCFAB" }; 
char *passwords[1] = { "TPPDMCMDRD" };

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

TheWifi::TheWifi(int networkId, Tiny_SH1106 *lcd)
{
    _lcd = lcd;

    _timer = new Interval();
    _timer->Start(1000, true);

//    WiFi.mode(WIFI_STA);
//    WiFi.disconnect();
//    delay(100);

    _networkId = networkId;

    WiFi.begin(ssids[_networkId], passwords[_networkId]);
}

TheWifi::~TheWifi()
{
    delete _timer;
}

void TheWifi::Render()
{
    if (!_timer->Ready())
      return;

    _lcd->fillRect(120,0, 8, 8, BLACK);

    if (WiFi.status() != WL_CONNECTED)
    {
        _lcd->drawBitmap(110, 0, wifiOff, 8, 8, 1);
        Serial.println("Not connected");
    }
    else
    {
        switch (GetStrength())
        {
            case 0: _lcd->drawBitmap(120, 0, wifiLow, 8, 8, 1); break;
            case 1: _lcd->drawBitmap(120, 0, wifiMedium, 8, 8, 1); break;
            case 2: _lcd->drawBitmap(120, 0, wifiHigh, 8, 8, 1); break;
            default: _lcd->drawBitmap(120, 0, wifiFull, 8, 8, 1); break;
        }
    }

    _lcd->display();
}

bool TheWifi::IsConnected()
{
    if (WiFi.status() == WL_CONNECTED)
        return true;
    else
        return false;
}

int TheWifi::GetStrength()
{
    int rssi = WiFi.RSSI();
    if (rssi < -80)
        return 0;

    if (rssi < -70)
        return 1;

    if (rssi < -60)
        return 2;

    return 3;
}

const __FlashStringHelper *TheWifi::GetStatus()
{
    switch (WiFi.status())
    {
        case WL_CONNECTED: return F("Connected");
        case WL_NO_SHIELD: return F("No Shield");
        case WL_IDLE_STATUS: return F("Idle");
        case WL_NO_SSID_AVAIL: return F("No SSID");
        case WL_SCAN_COMPLETED: return F("Scan Complete");
        case WL_CONNECT_FAILED: return F("Failed");
        case WL_CONNECTION_LOST: return F("Lost");
        case WL_DISCONNECTED: return F("Disconnected");
    }
}

String TheWifi::GetSSID()
{
    return WiFi.SSID();
}

int TheWifi::GetRSSI()
{
    return WiFi.RSSI();
}

IPAddress TheWifi::GetIP()
{
    return WiFi.localIP();
}

uint32_t TheWifi::GetNtpTime()
{
    struct tm timeinfo;

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return 0;
    }

    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");    

    time_t rawtime = mktime(&timeinfo);
    Serial.println(rawtime);

    return 0;
}

void TheWifi::SetNtpTime(ClockTime *clockTime)
{
    struct tm timeinfo;

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }

    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");    

    clockTime->SetTime(timeinfo);
}