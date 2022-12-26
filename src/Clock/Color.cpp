/*
  Color.h -
  Created by Florian Adam, 2021-12-29.
*/

#include "Color.h"

Color::Color()
    : m_color(0x00000000)
{
}

Color::~Color()
{
}

Color::Color(ColorPartType red, ColorPartType green, ColorPartType blue)
{
  m_color = (ColorIntType)red << 16 | ((ColorIntType)green << 8) | blue;
}

Color::Color(ColorIntType colorValue)
    : m_color(colorValue)
{
}

Color::ColorIntType Color::getRGBValue() const
{
  return m_color;
}

Color::ColorPartType Color::getRed() const
{
  return (m_color >> 16) & 0xFF;
}

Color::ColorPartType Color::getGreen() const
{
  return (m_color >> 8) & 0xFF;
}

Color::ColorPartType Color::getBlue() const
{
  return m_color & 0xFF;
}

bool Color::operator==(Color const &rhs)
{
  return m_color == rhs.m_color;
}

bool Color::operator!=(Color const &rhs)
{
  return !(*this == rhs);
}
