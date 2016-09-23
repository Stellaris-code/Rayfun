/*  src/hud/spinningmedal.cpp SpinningMedal - Yann BOUCHER (yann) 01/09/2016
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
#include "hud/spinningmedal.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Rayfun
{

SpinningMedal::SpinningMedal(const sf::Texture &t_medalTexture)
    : m_medalSprite(t_medalTexture), m_medal(m_medalSprite)
{
}

void SpinningMedal::update(const sf::Time &t_dt)
{
    m_spinAmount += t_dt.asSeconds() * speed;
    m_medal.spin(m_spinAmount);
}

void SpinningMedal::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_medal, t_states);
}

}
