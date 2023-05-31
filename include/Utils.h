#ifndef UTILS_H
#define UTILS_H

// should I namespace this?

struct HSL
{
    int hue, saturation, lightness;
};

struct RGB
{
    uint8_t red, green, blue;
};

HSL rgb2hsl(double red, double green, double blue);
HSL calculateAverageHSL(const std::vector<HSL> &colors);
RGB calculateAverageRGB(const std::vector<RGB> &colors);
bool areFirstTwoPixelsDifferentFromLastTwoPixels(const std::vector<RGB> &colors);
bool isColorCloseEnoughToReference(HSL reference, HSL other);
bool isColorCloseEnoughToReferenceRGB(RGB reference, RGB other, bool beStrict);
#endif
