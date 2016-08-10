/*  src/hud.cpp HUD - Yann BOUCHER (yann) 20/06/2016
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
#include "hud/hud.hpp"

#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>

#include "weapon.hpp"

namespace Rayfun
{

HUD::HUD(const Resources &t_res) :
    m_res(t_res)
{
    m_lifeArmor.setFont(t_res.fontHolder["doom"]);
    //m_lifeArmor.setColor(sf::Color::Red);
    m_lifeArmor.setCharacterSize(15);
    m_lifeArmor.setString("Life : 100\nArmor : 100");

    m_ammoList.setFont(t_res.fontHolder["doom"]);
    m_ammoList.setCharacterSize(15);
    m_ammoList.setString("Bullets : 10");
    updatePos();
}

void HUD::setWeapon(Weapon* t_weapon)
{
    m_weapon = t_weapon;
    m_weapon->setResources(m_res);
    updatePos();
}

void HUD::setSize(const sf::Vector2d &t_size)
{
    m_size = t_size;
    updatePos();
}

void HUD::setPlayerInfo(const HUD::PlayerInfo &t_plyInfo)
{
    m_lifeArmor.setString("Life : " + std::to_string(t_plyInfo.health) + "\nArmor : " +
                          std::to_string(t_plyInfo.armor));
    sf::String ammoStr;
    for (auto pair : t_plyInfo.ammo)
    {
        ammoStr += pair.first + " : " + std::to_string(pair.second) + "\n";
    }
    m_ammoList.setString(ammoStr);
}

void HUD::updateBob(double t_amount)
{
    if (t_amount == 0)
    {
        updatePos();
    }
    else
    {
        m_weapon->move((sin(m_runningTime + t_amount) - sin(m_runningTime)) * 100, 0);
    }
    m_runningTime += t_amount;
}

void HUD::update(sf::Time t_deltaTime)
{
    updateBob(t_deltaTime.asSeconds() * moveSpeed);
}

void HUD::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    if (m_weapon)
    {
        t_target.draw(*m_weapon, t_states);
    }
    t_target.draw(m_lifeArmor);
    t_target.draw(m_ammoList);
}

void HUD::updatePos()
{
    if (m_weapon)
    {
        m_weapon->setPosition(size().x / 2, 0);
    }
    m_lifeArmor.setPosition(size().x / 100, size().y - m_lifeArmor.getLocalBounds().height - size().y / 100);
    m_ammoList.setPosition(size().x - m_ammoList.getLocalBounds().width - size().x / 100,
                           size().y - m_ammoList.getLocalBounds().height - size().y / 100);
}


}
