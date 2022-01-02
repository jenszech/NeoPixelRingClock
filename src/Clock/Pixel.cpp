/*
  Pixel.h - 
  Created by Florian Adam, 2021-12-29.
*/

#include "Pixel.h"

Pixel::Pixel()
: m_status(PixelStatus::STATIC_COLOR)
{

}

Pixel::~Pixel() 
{

}

Pixel::Pixel(Color const &defaultColor) 
: m_defaultColor(defaultColor), m_status(PixelStatus::STATIC_COLOR), m_fadeValue(0)
{
    m_sourceColor = defaultColor;
    m_destinationColor = defaultColor;
}

void Pixel::setColor(Color const &newColor)
{
    if(m_destinationColor != newColor) {
        m_sourceColor = m_destinationColor;
        m_destinationColor = newColor;
        m_fadeValue = 0;
        m_status = PixelStatus::FADING;
    }
}

void Pixel::setDefaultColor()
{
    m_sourceColor = m_destinationColor;
    m_destinationColor = m_defaultColor;
    m_fadeValue = 0;
    m_status = PixelStatus::FADING;
}

Color Pixel::colorValueLoop()
{
    switch (m_status)
    {
        case PixelStatus::STATIC_COLOR:
            return m_destinationColor;
        case PixelStatus::FADING:
        {   
            m_fadeValue++;
            if(m_fadeValue >= 254) {
                m_status = PixelStatus::STATIC_COLOR;
                m_sourceColor = m_destinationColor;
                return m_destinationColor;
            }
            return getFadedColorValue(m_fadeValue, m_sourceColor, m_destinationColor);
        }
        default:
            m_status = PixelStatus::STATIC_COLOR;
            return m_destinationColor;
    }
}

Color Pixel::getFadedColorValue(uint8_t crossfade, Color const &from, Color const& to)
{
    auto redSrc = from.getRed();
    auto greenSrc = from.getGreen();
    auto blueSrc = from.getBlue();
    auto redDst = to.getRed();
    auto greenDst = to.getGreen();
    auto blueDst = to.getBlue();
    auto red = redSrc + ((redDst - redSrc) * crossfade / 256);
    auto green = greenSrc + ((greenDst - greenSrc) * crossfade / 256);
    auto blue = blueSrc + ((blueDst - blueSrc) * crossfade / 256);

    return Color(red, green, blue);
}
