#ifndef _REMOTE_STATION_H
#define _REMOTE_STATION_H

#include "WiFi.h"
#include "AsyncUDP.h"
#include "src/hardware/Interval.h"

#define UDP_LISTEN_PORT 8888

class RemoteStation
{
  private:
    Interval *_connectInterval;
    AsyncUDP *_udp;

    
  public: 
    bool Connected = false;

    RemoteStation();
    ~RemoteStation();
    void Connect();

}; 
#endif