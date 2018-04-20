#include <Arduino.h>
#include <Wire.h>

#include <NeoMaple.h>
#include "vmath.h"
#include "sensor.h"

int ESPI2CAddress = 0x53;

#define NUMPIXELS 16

NeoMaple pixels = NeoMaple(NUMPIXELS, NEO_GRB + NEO_KHZ800);

int dir = 0;
int focus = 20;

int delayval = 100;
double previousRotation[3];

void setup()
{
    Serial.begin(9600);
    pixels.begin();

    delay(100);
    Wire.begin();

    initSensor();

    Serial.println("Setup done");
}

double calculateDistance(int dotIndex, double normal[3])
{
    double offset[3] = {0, 0, .5F};
    double dot[3] = {
        sin((double)dotIndex / (double)NUMPIXELS * PI * 2.0),
        cos((double)dotIndex / (double)NUMPIXELS * PI * 2.0),
        0};

    Serial.print("DOT");
    Serial.print(dotIndex);
    Serial.print(" X= ");
    Serial.print(dot[0]);
    Serial.print(" Y= ");
    Serial.print(dot[1]);
    Serial.print(" Z= ");
    Serial.println(dot[2]);

    return normal[0] * (dot[0] + offset[0]) +
           normal[1] * (dot[1] + offset[1]) +
           normal[2] * (dot[2] + offset[2]);
}

double calculateIntensity(double distance)
{
    return floor((1 - distance) * 255);
}

void updateDot(int dotIndex, double intensity)
{
    pixels.setPixelColor(dotIndex, pixels.Color(0, intensity, 0));
}

void updateDots(double normal[3])
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        double distance = calculateDistance(i, normal);
        double intensity = calculateIntensity(distance);
        updateDot(i, intensity);
    }
}

void eulerToVector(float rotation[3], double normal[3])
{
    double roll = rotation[0] * PI / 180;
    double yaw = rotation[1] * PI / 180;
    double pitch = rotation[2] * PI / 180;

    normal[0] = -sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll);
    normal[1] = -cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll);
    normal[2] = cos(pitch) * cos(roll);
}

void loop()
{
    /*Wire.requestFrom(ESPI2CAddress, 3);

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
    Serial.println(Z);*/

    float rotation[3];
    double normal[3];
    readSensor(rotation);

    if (rotation[0] != previousRotation[0]
        && rotation[1] != previousRotation[1]
        && rotation[2] != previousRotation[2])
    {
        previousRotation[0] = rotation[0];
        previousRotation[1] = rotation[1];
        previousRotation[2] = rotation[2];

        eulerToVector(rotation, normal);

        Serial.print("X= ");
        Serial.print(normal[0]);
        Serial.print(" Y= ");
        Serial.print(normal[1]);
        Serial.print(" Z= ");
        Serial.println(normal[2]);

        updateDots(normal);

        pixels.show();
    }

    delay(delayval);
}
