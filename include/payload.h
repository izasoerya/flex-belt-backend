#pragma once

#include <Arduino_JSON.h>

class Payload
{
private:
    float angleX;
    float angleY;
    float angleZ;
    uint16_t flex;
    String description;
    String status;

public:
    Payload(float x, float y, float z, uint16_t f, String d = "", String s = "")
        : angleX(x), angleY(y), angleZ(z), flex(f), description(d), status(s) {}

    JSONVar toJSON()
    {
        JSONVar payloadData;
        payloadData["angleX"] = angleX;
        payloadData["angleY"] = angleY;
        payloadData["angleZ"] = angleZ;
        payloadData["flex"] = flex;
        payloadData["description"] = description;
        payloadData["status"] = status;

        return payloadData;
    }
};