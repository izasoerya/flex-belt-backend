#pragma once

#include "BluetoothSerial.h"
#include "payload.h"
#include "actuator.h"

class BluetoothClient
{
private:
    BluetoothSerial &bluetooth;

public:
    BluetoothClient(BluetoothSerial &bt) : bluetooth(bt) {};

    ~BluetoothClient() { bluetooth.end(); };

    bool begin(const char *deviceName) { return bluetooth.begin(deviceName); };

    bool isConnected() { return bluetooth.connected(); };

    void send(Payload payload)
    {
        if (bluetooth.connected())
        {
            JSONVar json = payload.toJSON();
            String jsonString = JSON.stringify(json);
            bluetooth.println(jsonString);
        }
        else
        {
            Serial.println("Bluetooth not connected");
        }
    };

    void receive(HardwareSerial &Serial)
    {
        if (bluetooth.available())
        {
            String receivedData = bluetooth.readStringUntil('\n');
            if (receivedData.length() > 0)
            {
                Serial.print("Received Data: ");
                Serial.println(receivedData);
                
                Serial.print("[DEBUG] Raw receivedData: ");
                for (int i = 0; i < receivedData.length(); i++) {
                  Serial.print((int)receivedData[i]); Serial.print(" ");
                }
                Serial.println();
                JSONVar parsed = JSON.parse(receivedData);
                Actuator data;
                data.fromJSON(parsed);

                uint16_t heater;
                bool isCold;
                data.getData(heater, isCold);

                Serial.print("Parsed Actuator Data: ");
                Serial.print("Heater: ");
                Serial.print(heater);
                Serial.print(", IsCold: ");
                Serial.println(isCold);
            }
        }
    };
};