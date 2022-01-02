/*
  Clock.h - 
  Created by Florian Adam, 2022-01-01.
*/

#ifndef Clock_h
#define Clock_h

#include "PixelManager.h"

class Clock 
{
  public:
    Clock(uint16_t numPixel, uint8_t pin);

    virtual ~Clock();

    void initialize();
    void updateLoop();

  private:
    PixelManager m_pixelManager;
    Color m_secondsColor;
    Color m_minutesColor;
    Color m_hoursColor;

    uint8_t m_previousSecond = 0;
    uint8_t m_previousMinute = 0;
    uint8_t m_previousHour = 0;

    void updateHour(time_t timestamp);
    void updateMinute(time_t timestamp);
    void updateSecond(time_t timestamp);
};

#endif //Clock_h