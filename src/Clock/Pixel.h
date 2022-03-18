/*
  Pixel.h - 
  Created by Florian Adam, 2021-12-29.
*/

#ifndef Pixel_h
#define Pixel_h

#include "Color.h"

class Pixel
{
public:
    enum class PixelStatus:uint8_t
    {
        STATIC_COLOR = 0,
        FADING
    };

    Pixel();
    Pixel(Color const &defaultColor);

    virtual ~Pixel();

    void setColor(Color const &newColor);
    void setDefaultColor();

    Color colorValueLoop();

private:
    const Color m_defaultColor;
    Color m_sourceColor;
    Color m_destinationColor;

    PixelStatus m_status;
    uint8_t m_fadeValue = 0;

    static Color getFadedColorValue(uint8_t crossfade, Color const &from, Color const& to);
    static int interpolate(uint8_t startValue, uint8_t endValue, uint8_t stepNumber);
};

#endif //Pixel_h
