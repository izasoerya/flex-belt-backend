#pragma once

#include <Arduino.h>
#include "payload.h"

class DSS
{
private:
    Payload payload;

public:
    DSS(Payload payload) : payload(payload) {}

    String generateDescription()
    {
        float x = payload.getAngleX();
        float y = payload.getAngleY();
        uint16_t flex = payload.getFlex();

        if (x > 10 && y > -50 && y < -30 && flex > 580)
        {
            return "Normal";
        }
        else if (x < 8 && y > -30 && flex > 600)
        {
            return "Skoliosis Kiri";
        }
        else if (x < 8 && y < -50 && flex < 580)
        {
            return "Skoliosis Kanan";
        }
        else if (x > 12 && y > -50 && y < -30 && flex < 580)
        {
            return "Kifosis";
        }
        else if (x < 5 && y > 30 && flex > 600)
        {
            return "Lordosis";
        }
        else
        {
            return "Normal";
        }
    }

    String generateStatus()
    {
        // Generate the description
        String description = generateDescription();

        // Determine the status based on the description and angleX
        if (description == "Normal")
        {
            return "safe";
        }
        else if (payload.getAngleX() < 45)
        {
            return "caution";
        }
        else
        {
            return "dangerous";
        }
    }
};