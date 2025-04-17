#pragma once

#include <Arduino_JSON.h>

class Actuator
{
private:
    uint16_t heater;
    bool isCold;

public:
    Actuator(uint16_t v = 0, bool c = false) : heater(v), isCold(c) {}

    JSONVar toJSON()
    {
        JSONVar actuatorData;
        actuatorData["heater"] = heater;
        actuatorData["isCold"] = isCold;

        return actuatorData;
    }

    Actuator fromJSON(const JSONVar &json)
    {
        if (json.hasOwnProperty("heater"))
        {
            heater = json["heater"];
        }
        if (json.hasOwnProperty("isCold"))
        {
            isCold = json["isCold"];
        }
        return *this;
    }

    void getData(uint16_t &v, bool &c)
    {
        v = heater;
        c = isCold;
    }
};