#include <Arduino.h>
#include <Wire.h>

#include <NeoMaple.h>
#include "vmath.h"

int ESPI2CAddress = 0x53;

#define NUMPIXELS 16

NeoMaple pixels = NeoMaple(NUMPIXELS, NEO_GRB + NEO_KHZ800);

int dir = 0;
int focus = 20;

int delayval = 100;
float dots[NUMPIXELS][3];
float normal[3];

void setup()
{
    Serial.begin(9600);
    pixels.begin();

    delay(100);

    for (int i = 0; i < NUMPIXELS; i++)
    {
        dots[i][0] = sin(i / NUMPIXELS * PI * 2);
        dots[i][1] = cos(i / NUMPIXELS * PI * 2),
        dots[i][2] = 0;
    }
}

float calculateDistance(float dot[3])
{
    float offset[3] = { 0, 0, .5F };
    return dotProduct(normal, add(dot, offset));
}

float calculateIntensity(float distance)
{
    return floor((1 - distance) * 255);
}

void updateDot(int dotIndex, float intensity)
{
    pixels.setPixelColor(dotIndex, pixels.Color(0, intensity, 0));
}

void updateDots()
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        float distance = calculateDistance(dots[i]);
        float intensity = calculateIntensity(distance);
        updateDot(i, intensity);
    }
}

void loop()
{
    Wire.requestFrom(ESPI2CAddress, 3);

    int X, Y, Z;
    if (Wire.available() <= 3)
    {
        X = Wire.read();
        Y = Wire.read();
        Z = Wire.read();
    }

    Serial.print("X= ");
    Serial.print(X);
    Serial.print(" Y= ");
    Serial.print(Y);
    Serial.print(" Z= ");
    Serial.println(Z);

    updateDots();

    pixels.show();

    delay(delayval);
}
