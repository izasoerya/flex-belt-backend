#pragma once

#include <Arduino.h>

#define LPWM 18
#define RPWM 19
#define REN 5
#define LEN 17

#define MAX_SPEED 255

class Motor
{
private:
    bool isCold = true;
    byte heater = 0;

public:
    Motor() {};

    void begin()
    {
        pinMode(LPWM, OUTPUT);
        pinMode(RPWM, OUTPUT);
        pinMode(REN, OUTPUT);
        pinMode(LEN, OUTPUT);

        digitalWrite(REN, HIGH);
        digitalWrite(LEN, HIGH);
    };

    void setConfig(bool isCold, byte heater)
    {
        this->isCold = isCold;
        this->heater = heater;
    };

    void setSpeed()
    {
        if (isCold)
        {
            uint16_t heaterSpeed = map(heater, 0, 100, 0, MAX_SPEED);
            analogWrite(LPWM, 0);
            digitalWrite(LPWM,LOW);            
            analogWrite(RPWM, heaterSpeed);
        }
        if (!isCold)
        {
            uint16_t heaterSpeed = map(heater, 0, 100, 0, MAX_SPEED);
            analogWrite(RPWM, 0);
            digitalWrite(RPWM,LOW);
            analogWrite(LPWM, heaterSpeed);
        }
    }
};
