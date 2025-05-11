#pragma once

#include <Arduino.h>
#include "payload.h"

 uint8_t counterLord = 0;
 uint8_t counterKif = 0;
 uint8_t counterSKn = 0;
 uint8_t counterSKr = 0;
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
        float z = payload.getAngleZ();
        uint16_t flex = payload.getFlex();

        // Serial.println("KIFOSIS: " + String(counterKif));
        // Serial.println("LORDOSIS: " + String(counterLord));
        // Serial.println("SKOLIOSIS KIRI: " + String(counterSKn));
        // Serial.println("SKOLIOSIS KANAN: " + String(counterSKr));

        // if (x < 8 && y > -30 && y < 0 && flex > 600)
        // {
        //     return "Skoliosis Kiri";
        // }
        // else if (x < 8 && y < -50 && flex < 580)
        // {
        //     return "Skoliosis Kanan";
        // }
        // else if (x > 12 && y > -50 && y < -30 && flex < 580)
        // {
        //     return "Kifosis";
        // }
        // else if (x < 5 && y > 30 && flex > 600)
        // {
        //     return "Lordosis";
        // }
        if (z < 60 && y>=-30 && y<30) {
            if (counterKif > 3) {
                return "Kifosis";
            }
            counterKif++;
            return "Normal";
        } else if (z > 85&&y>=-30 && y<30) {
            if (counterLord > 3) {
                return "Lordosis";
            }
            counterLord++;
            return "Normal";
        } else if (y < -30) {
            if (counterSKr > 3) {
                return "Skoliosis Kanan";
            }
            counterSKr++;
            return "Normal";
        } else if (y > 30) {
            if (counterSKn > 3) {
                return "Skoliosis Kiri";
            }
            counterSKn++;
            return "Normal";
        } 
        else
        {
            counterKif = 0;
            counterLord = 0;
            counterSKn = 0;
            counterSKr = 0;
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