/*  include/hud/textcursor.hpp TextCursor - Yann BOUCHER (yann) 21/09/2016
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
#ifndef TEXTCURSOR_HPP
#define TEXTCURSOR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/System/Time.hpp>

namespace Rayfun
{

class TextCursor : public sf::Drawable, public sf::Transformable
{
    public:
        TextCursor();

    public:
        void setThickness(float val);
        void setHeight(float val);
        void setColor(sf::Color color);

        float thickness() const;
        float height() const;
        sf::Color color() const;

        void update(sf::Time dt);

        void resetBlink();

    public:
        sf::Time blinkRate = sf::seconds(0.5);

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        sf::RectangleShape m_cursor;
        sf::Time m_counter;
        bool m_showCursor { true };
};

}

#endif // TEXTCURSOR_HPP
