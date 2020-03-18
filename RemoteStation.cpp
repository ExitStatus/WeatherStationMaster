#include "RemoteStation.h"

RemoteStation::RemoteStation()
{
    _udp = new AsyncUDP();
    _connectInterval = new Interval(1000, true);
}

void RemoteStation::Connect()
{
    if (_connectInterval == NULL || !_connectInterval->Ready())
        return;

    if (WiFi.status() != WL_CONNECTED)
        return;

    if (_udp->listen(UDP_LISTEN_PORT))
    {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());

        _udp->onPacket([](AsyncUDPPacket packet)
        {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
        });

        delete _connectInterval;
        _connectInterval = NULL;

        Connected = true;
    }    
}

RemoteStation::~RemoteStation()
{
    delete _udp;

    if (_connectInterval != NULL)
        delete _connectInterval;
}