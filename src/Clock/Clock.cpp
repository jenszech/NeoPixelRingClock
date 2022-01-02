/*
  Clock.h - 
  Created by Florian Adam, 2022-01-01.
*/

#include "Clock.h"
#include <TimeLib.h>

Clock::Clock(uint16_t numPixel, uint8_t pin)
: m_pixelManager(numPixel, pin)
{
  //TODO configuration
  m_secondsColor = Color(50,0,0);
  m_minutesColor = Color(50,0,50);
  m_hoursColor   = Color(0,0,50);
  
  const Color quartersColor(0,30,0);
  const Color fivesColor(0,1,0);
  const std::vector< std::pair<uint8_t, Color> > defaultColors {
      {0, quartersColor},
      {5, fivesColor},
      {10, fivesColor},
      {15, quartersColor},
      {20, fivesColor},
      {25, fivesColor},
      {30, quartersColor},
      {35, fivesColor},
      {40, fivesColor},
      {45, quartersColor},
      {50, fivesColor},
      {55, fivesColor} 
  };

  m_pixelManager.setDefaultPixelColors(defaultColors);
}

Clock::~Clock()
{}

void Clock::initialize()
{
  m_pixelManager.initialize();
}

void Clock::updateLoop()
{
  time_t currentTimestamp = now();

  updateHour(currentTimestamp);
  updateMinute(currentTimestamp);
  updateSecond(currentTimestamp);

  m_pixelManager.updateLoop();
}

void Clock::updateHour(time_t timestamp)
{
  uint8_t curHour = hour(timestamp);
  uint8_t curMin = minute(timestamp);

  curHour = (curHour * 5) + (curHour / 12) + (curMin / 12);
  if(m_previousHour != curHour) {
      m_pixelManager.resetPixelColor(m_previousHour);
      m_previousHour = curHour;
  }
  m_pixelManager.setPixelColor(m_hoursColor, curHour);
}

void Clock::updateMinute(time_t timestamp)
{
  uint8_t curMin = minute(timestamp);
  
  if(m_previousMinute != curMin) {
      m_pixelManager.resetPixelColor(m_previousMinute);
      m_previousMinute = curMin;
  }
  m_pixelManager.setPixelColor(m_minutesColor, curMin);
}

void Clock::updateSecond(time_t timestamp)
{
  uint8_t curSec = second(timestamp);
  uint8_t curMin = minute(timestamp);

  if(m_previousSecond != curSec) {
    if(m_previousSecond != curMin && m_previousHour != curMin) {
        m_pixelManager.resetPixelColor(m_previousSecond);
    }
    m_previousSecond = curSec;
  }
  m_pixelManager.setPixelColor(m_secondsColor, curSec);
}
