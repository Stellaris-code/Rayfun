/*  src/hud/textcursor.cpp TextCursor - Yann BOUCHER (yann) 21/09/2016
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/
#include "hud/textcursor.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace Rayfun
{


TextCursor::TextCursor()
{
    setThickness(2);
    setHeight(10);
    setColor(sf::Color::White);
}

void TextCursor::setThickness(float val)
{
    m_cursor.setSize({val, height()});
}

void TextCursor::setHeight(float val)
{
    m_cursor.setSize({thickness(), val});
}

void TextCursor::setColor(sf::Color color)
{
    m_cursor.setFillColor(color);
}

float TextCursor::thickness() const
{
    return m_cursor.getSize().x;
}

float TextCursor::height() const
{
    return m_cursor.getSize().y;
}

sf::Color TextCursor::color() const
{
    return m_cursor.getFillColor();
}

void TextCursor::update(sf::Time dt)
{
    m_counter += dt;
    if (m_counter >= blinkRate)
    {
        m_showCursor = !m_showCursor;
        m_counter = sf::Time::Zero;
    }
}

void TextCursor::resetBlink()
{
    m_showCursor = true;
    m_counter = sf::Time::Zero;
}

void TextCursor::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    if (m_showCursor)
    {
        t_target.draw(m_cursor, t_states);
    }
}

}
