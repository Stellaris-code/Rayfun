/*  include/animations/moveanimation.hpp MoveAnimation - Yann BOUCHER (yann) 31/08/2016
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
#ifndef MOVEANIMATION_HPP
#define MOVEANIMATION_HPP

#include <SFML/System/Vector2.hpp>

namespace Rayfun
{

class MoveAnimation
{
    public:

        MoveAnimation(const sf::Vector2f& from, const sf::Vector2f& to)
        {
            setParams(from, to);
        }

        void setParams(const sf::Vector2f& from, const sf::Vector2f& to)
        {
            m_from = from;
            m_direction = to - from;
        }

        template <typename Animated>
        void operator()(Animated& animated, float progress){
            auto pos = m_from + progress * m_direction;
            animated.setPosition(pos);
        }

    private:
        sf::Vector2f m_from;
        sf::Vector2f m_direction;
};

}

#endif // MOVEANIMATION_HPP
