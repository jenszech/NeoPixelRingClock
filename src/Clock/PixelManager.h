/*
  PixelManager.h -
  Created by Florian Adam, 2021-12-29.
*/

#ifndef PixelManager_h
#define PixelManager_h

#include "Pixel.h"

#include <Adafruit_NeoPixel.h>

#include <vector>
#include <map>

class PixelManager
{
public:
  PixelManager(uint16_t numPixel, uint8_t pin);
  virtual ~PixelManager();

  void initialize();

  void updateLoop();

  void setDefaultPixelColors(std::vector<std::pair<uint8_t, Color>> const &defaultColors);

  void setHourPixel(Color const &color, uint8_t pixelIndex);
  void setMinutePixel(Color const &color, uint8_t pixelIndex);
  void setSecondPixel(Color const &color, uint8_t pixelIndex);

  void setGlobalBrightness(uint8_t level);

private:
  std::map<uint8_t, Pixel> m_pixels;

  Adafruit_NeoPixel m_neoPixels;
};

#endif // PixelManager_h
