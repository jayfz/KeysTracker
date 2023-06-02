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

RGB calculateAverageRGB(const std::vector<RGB> &colors);
bool areFirstTwoPixelsDifferentFromLastTwoPixels(const std::vector<RGB> &colors);
bool isColorCloseEnoughToReferenceRGB(RGB reference, RGB other, bool beStrict);
uint8_t stringHexToInt(const std::string &colorChannel);
RGB stringToRGB(char *color);
#endif
