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
        Serial.println(JSONVar().stringify(json)); // <== Add this to see the JSON data
        if (json.hasOwnProperty("heater"))
        {
            heater = static_cast<int>(json["heater"]); // <== force cast
        }
        if (json.hasOwnProperty("isCold"))
        {
            isCold = static_cast<bool>(json["isCold"]);
        }
        return *this;
    }

    Actuator fromRawJson(const String &jsonStr)
    {
        int heaterStart = jsonStr.indexOf("\"heater\":") + 9;
        int heaterEnd = jsonStr.indexOf(',', heaterStart);
        String heaterValue = jsonStr.substring(heaterStart, heaterEnd);
        heater = heaterValue.toInt();

        int isColdStart = jsonStr.indexOf("\"isCold\":") + 9;
        int isColdEnd = jsonStr.indexOf('}', isColdStart);
        String isColdValue = jsonStr.substring(isColdStart, isColdEnd);
        isColdValue.trim(); // clean up whitespace

        isCold = (isColdValue == "true");

        return *this;
    }

    void getData(uint16_t &v, bool &c)
    {
        v = heater;
        c = isCold;
    }
};