/*
  PixelColor.h - 
  Created by Florian Adam, 2021-12-29.
*/

#ifndef Color_h
#define Color_h

#include <stdint.h>

class Color 
{
  public:
    typedef uint8_t ColorPartType;
    typedef uint32_t ColorIntType;

    Color();
    Color(ColorPartType const red, ColorPartType const green, ColorPartType const blue);
    Color(ColorIntType const colorValue);

    virtual ~Color();

    ColorIntType getRGBValue() const;
    ColorPartType getRed() const;
    ColorPartType getGreen() const;
    ColorPartType getBlue() const;

    bool operator== (Color const &rhs);
    bool operator!= (Color const &rhs);

  private:
    ColorIntType m_color;
};

#endif //Color_h