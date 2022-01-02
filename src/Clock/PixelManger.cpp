/*
  PixelManager.cpp - 
  Created by Florian Adam, 2021-12-29.
*/

#include "PixelManager.h"

PixelManager::PixelManager(uint16_t numPixel, uint8_t pin)
: m_neoPixels(numPixel, pin, NEO_GRB + NEO_KHZ800)
{
    
}

PixelManager::~PixelManager()
{
    m_pixels.clear();
}

void PixelManager::setDefaultPixelColors(std::vector<std::pair<uint8_t, Color> > const &defaultColors)
{
    m_pixels.clear();
    for(auto entry:defaultColors)
    {
        m_pixels.emplace(entry.first, Pixel(entry.second));
    }
}

void PixelManager::initialize()
{
    m_neoPixels.begin();  // INITIALIZE NeoPixel strip object
}

void PixelManager::updateLoop()
{
    for(auto &entry:m_pixels)
    {
        Color newColor = entry.second.colorValueLoop();
        m_neoPixels.setPixelColor(entry.first, newColor.getRGBValue());
    }
    m_neoPixels.show();
}

void PixelManager::setPixelColor(Color const &destinationColor, uint8_t pixelIndex)
{
    m_pixels[pixelIndex].setColor(destinationColor);
}

void PixelManager::resetPixelColor(uint8_t pixelIndex)
{
    m_pixels[pixelIndex].setDefaultColor();
}

void PixelManager::setGlobalBrightness(uint8_t level)
{
    m_neoPixels.setBrightness(level);
}