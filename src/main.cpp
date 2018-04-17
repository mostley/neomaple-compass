#include <Arduino.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <NeoMaple.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
NeoMaple pixels = NeoMaple(NUMPIXELS, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

void setup()
{
    Serial.begin(9600);
    pixels.begin(); // This initializes the NeoPixel library.
}

int dir = 0;
int focus = 20;

void loop()
{
    int centerIndex = floor((NUMPIXELS - 1) / 2);
    int centerIndexOffset = dir - centerIndex;
    for (int i = 0; i < NUMPIXELS; i++)
    {
        int index = abs((i + centerIndexOffset) % NUMPIXELS);
        int distance = (abs(centerIndex - index) / (float)NUMPIXELS) * focus;

        pixels.setPixelColor(i, pixels.Color(0, distance, 0));
    }

    pixels.show();

    dir = (dir + 1) % NUMPIXELS;

    delay(delayval);
}
