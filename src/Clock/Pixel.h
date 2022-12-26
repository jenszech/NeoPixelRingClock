/*
  Pixel.h -
  Created by Florian Adam, 2021-12-29.
*/

#ifndef Pixel_h
#define Pixel_h

#include "Color.h"
#include <map>

class Pixel
{
public:
    enum class PixelStatus : uint8_t
    {
        STATIC_COLOR = 0,
        FADING
    };

    enum class ColorLayer : uint8_t
    {
        BOTTOM = 0,
        FIRST,
        SECOND,
        THIRD
    };

    Pixel();
    Pixel(Color const &defaultColor);

    virtual ~Pixel();

    void setColor(ColorLayer const layer, Color const &newColor);
    void resetLayer(ColorLayer const layer);

    Color colorValueLoop();

private:
    std::map<ColorLayer, Color> m_colors;
    Color m_sourceColor;
    Color m_currentColor;
    Color m_destinationColor;

    PixelStatus m_status;
    uint8_t m_fadeValue = 0;

    void updateDestinationColorValue();
    void updateCurrentColorValue();

    static int interpolate(uint8_t startValue, uint8_t endValue, uint8_t stepNumber);
};

#endif // Pixel_h
