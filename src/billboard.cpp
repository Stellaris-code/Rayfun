/*  src/billboard.cpp Billboard - Yann BOUCHER (yann) 14/06/2016
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
#include "billboard.hpp"

#include "mathutility.hpp"

namespace Rayfun
{

void Billboard::setImage(const sf::Image &t_img, double t_minAngle, double t_maxAngle)
{
    m_sprites.push_back({{Utility::normalizeAngle(t_minAngle), Utility::normalizeAngle(t_maxAngle)}, t_img});
}

sf::Image Billboard::renderImage() const
{
    sf::Image blank;
    blank.create(1, 1, sf::Color::Transparent);
    for (auto pair : m_sprites)
    {
        if (playerFacingAngle() >= pair.first.first && playerFacingAngle() <= pair.first.second)
        {
            return pair.second;
        }
    }
    return blank;
}

}
