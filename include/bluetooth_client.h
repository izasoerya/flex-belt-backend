#pragma once

#include "BluetoothSerial.h"
#include "payload.h"
#include "actuator.h"
#include "motor.h"

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

    void receive(HardwareSerial &Serial, Motor &motor)
    {
        if (bluetooth.available())
        {
            String received = bluetooth.readStringUntil('\n');
            if (received.length() > 0)
            {
                Actuator act;
                act.fromRawJson(received);

                uint16_t heater;
                bool isCold;
                act.getData(heater, isCold);
                motor.setConfig(isCold, heater);
                motor.setSpeed();

                Serial.print("Parsed manually -> Heater: ");
                Serial.print(heater);
                Serial.print(", IsCold: ");
                Serial.println(isCold);
            }
        }
    };
};