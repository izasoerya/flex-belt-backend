#pragma once

#include <Arduino.h>
#include "BTS7960.h"

#define MAX_SPEED 255

class Motor
{
private:
    const uint8_t LPWM = 18;
    const uint8_t RPWM = 19;
    const uint8_t REN = 5;
    const uint8_t LEN = 17;

    BTS7960 *motorController = nullptr;

    bool isCold = true;
    byte heater = 0;

public:
    Motor() {}

    void begin()
    {
        motorController = new BTS7960(LEN, REN, LPWM, RPWM);
        motorController->Enable();
    }

    void setConfig(bool isCold, byte heater)
    {
        this->isCold = isCold;
        this->heater = heater;
    }

    void setSpeed()
    {
        if (!motorController)
            return;

        uint16_t heaterSpeed = map(heater, 0, 100, 0, MAX_SPEED);

        if (isCold)
        {
            motorController->TurnRight(heaterSpeed);
        }
        else
        {
            motorController->TurnLeft(heaterSpeed);
        }

        delayMicroseconds(100);
    }

    void stop()
    {
        if (motorController)
        {
            motorController->Stop();
            motorController->Disable();
        }
    }

    ~Motor()
    {
        stop();
        delete motorController;
    }
};
