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
        if (payload.getAngleY() <= -70)
            return "Indikasi Lordosis";
        else if (payload.getAngleY() < -60 && payload.getAngleY() > -70)
            return "Indikasi Kyphosis";
        else if (payload.getAngleX() > 15)
            return "Indikasi Skoliosis Kiri";
        else
            return "Normal";
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