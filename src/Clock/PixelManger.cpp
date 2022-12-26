/*
  PixelManager.cpp -
  Created by Florian Adam, 2021-12-29.
*/

#include "PixelManager.h"

static const Pixel::ColorLayer HOUR_LAYER = Pixel::ColorLayer::FIRST;
static const Pixel::ColorLayer MINUTE_LAYER = Pixel::ColorLayer::SECOND;
static const Pixel::ColorLayer SECOND_LAYER = Pixel::ColorLayer::THIRD;

PixelManager::PixelManager(uint16_t numPixel, uint8_t pin)
    : m_neoPixels(numPixel, pin, NEO_GRB + NEO_KHZ800)
{
}

PixelManager::~PixelManager()
{
    m_pixels.clear();
}

void PixelManager::setDefaultPixelColors(std::vector<std::pair<uint8_t, Color>> const &defaultColors)
{
    m_pixels.clear();
    for (auto entry : defaultColors)
    {
        m_pixels.emplace(entry.first, Pixel(entry.second));
    }
}

void PixelManager::initialize()
{
    m_neoPixels.begin(); // INITIALIZE NeoPixel strip object
}

void PixelManager::updateLoop()
{
    for (auto &entry : m_pixels)
    {
        Color newColor = entry.second.colorValueLoop();
        m_neoPixels.setPixelColor(entry.first, newColor.getRGBValue());
    }
    m_neoPixels.show();
}

void PixelManager::setHourPixel(Color const &color, uint8_t pixelIndex)
{
    m_pixels[pixelIndex].setColor(HOUR_LAYER, color);
    for (auto &entry : m_pixels)
    {
        if (entry.first != pixelIndex)
        {
            entry.second.resetLayer(HOUR_LAYER);
        }
    }
}

void PixelManager::setMinutePixel(Color const &color, uint8_t pixelIndex)
{
    m_pixels[pixelIndex].setColor(MINUTE_LAYER, color);
    for (auto &entry : m_pixels)
    {
        if (entry.first != pixelIndex)
        {
            entry.second.resetLayer(MINUTE_LAYER);
        }
    }
}

void PixelManager::setSecondPixel(Color const &color, uint8_t pixelIndex)
{
    m_pixels[pixelIndex].setColor(SECOND_LAYER, color);
    for (auto &entry : m_pixels)
    {
        if (entry.first != pixelIndex)
        {
            entry.second.resetLayer(SECOND_LAYER);
        }
    }
}

void PixelManager::setGlobalBrightness(uint8_t level)
{
    m_neoPixels.setBrightness(level);
}