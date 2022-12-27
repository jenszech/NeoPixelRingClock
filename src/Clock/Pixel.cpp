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
    : m_status(PixelStatus::STATIC_COLOR), m_fadeValue(0)
{
    m_colors[ColorLayer::BOTTOM] = defaultColor;
    m_sourceColor = defaultColor;
    m_currentColor = defaultColor;
    m_destinationColor = defaultColor;
}

void Pixel::setColor(ColorLayer const layer, Color const &newColor)
{
    if (ColorLayer::BOTTOM != layer)
    {
        m_colors[layer] = newColor;
        updateDestinationColorValue();

        m_fadeValue = 0;
        m_status = PixelStatus::FADING;
    }
}

void Pixel::resetLayer(ColorLayer const layer)
{
    if (ColorLayer::BOTTOM != layer)
    {
        if (m_colors.erase(layer))
        {
            m_fadeValue = 0;
            m_status = PixelStatus::FADING;
        }
        updateDestinationColorValue();
    }
}

Color Pixel::colorValueLoop()
{
    if (PixelStatus::FADING == m_status)
    {
        m_fadeValue++;
        if (m_fadeValue >= 254)
        {
            m_status = PixelStatus::STATIC_COLOR;
            m_sourceColor = m_destinationColor;
            m_currentColor = m_destinationColor;
        }
        else
        {
            updateCurrentColorValue();
        }
    }

    return m_currentColor;
}

void Pixel::updateDestinationColorValue()
{

    Color::ColorPartType red = 0;
    Color::ColorPartType green = 0;
    Color::ColorPartType blue = 0;
    for (auto colorLayer : m_colors)
    {
        //Overwrite bottom (static) color layer, but mix other layers
        if(1 == m_colors.size() || Pixel::ColorLayer::BOTTOM != colorLayer.first)
        {
            red += colorLayer.second.getRed();
            green += colorLayer.second.getGreen();
            blue += colorLayer.second.getBlue();
        }
    }

    if (m_colors.size())
    {
        red = red / m_colors.size();
        blue = blue / m_colors.size();
        green = green / m_colors.size();
    }

    m_sourceColor = m_currentColor;
    m_destinationColor = Color(red, green, blue);
}

void Pixel::updateCurrentColorValue()
{
    auto redSrc = m_sourceColor.getRed();
    auto greenSrc = m_sourceColor.getGreen();
    auto blueSrc = m_sourceColor.getBlue();
    auto redDst = m_destinationColor.getRed();
    auto greenDst = m_destinationColor.getGreen();
    auto blueDst = m_destinationColor.getBlue();
    auto red = interpolate(redSrc, redDst, m_fadeValue);
    auto green = interpolate(greenSrc, greenDst, m_fadeValue);
    auto blue = interpolate(blueSrc, blueDst, m_fadeValue);

    m_currentColor = Color(red, green, blue);
}

int Pixel::interpolate(uint8_t startValue, uint8_t endValue, uint8_t stepNumber)
{
    return (endValue - startValue) * stepNumber / 255 + startValue;
}
