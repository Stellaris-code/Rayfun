/*  src/weapon.cpp Weapon - Yann BOUCHER (yann) 19/06/2016
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
#include "weapon.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "utils/gameplayglobals.hpp"

namespace Rayfun
{


Weapon::Weapon(unsigned long &t_ammopool, long t_magazineSize)
    : m_ammoPool(t_ammopool), m_magazineSize(t_magazineSize), m_animator(m_anims)
{

}

void Weapon::setSpriteSheet(const sf::Texture &t_spriteSheet)
{
    m_spriteSheet.setTexture(t_spriteSheet);
}

void Weapon::setFireAnimation(const thor::FrameAnimation &t_anim, const sf::Time &t_duration)
{
    m_anims.addAnimation("fire", t_anim, t_duration);
    m_fireAnimSet = true;
}

void Weapon::setReloadAnimation(const thor::FrameAnimation &t_anim, const sf::Time &t_duration)
{
    m_anims.addAnimation("reload", t_anim, t_duration);
    m_reloadAnimSet = true;
}

void Weapon::setIdleAnimation(const thor::FrameAnimation &t_anim, const sf::Time &t_duration)
{
    m_anims.addAnimation("idle", t_anim, t_duration);
    m_idleAnimSet = true;
}

void Weapon::setFireCallback(const std::function<void ()> &t_callback)
{
    m_fireCallback = t_callback;
}

void Weapon::setReloadCallback(const std::function<void ()> &t_callback)
{
    m_reloadCallback = t_callback;
}

void Weapon::reload()
{
    if (m_reloadAnimSet && checkCanReload())
    {
        if (!m_animPlaying)
        {
            m_animPlaying = true;
            m_animator.play() << "reload" <<
                                 thor::Playback::notify( [this]{
                m_animPlaying = false;
                reloadMagazine();
                if (m_reloadCallback)
                {
                    m_reloadCallback();
                }
            });
        }
    }
}

void Weapon::fire()
{
    if (checkCanFire())
    {
        if (m_fireAnimSet)
        {
            if (!m_animPlaying)
            {
                m_animPlaying = true;
                if (m_fireCallback)
                {
                    m_fireCallback();
                }
                fireAmmo();
                m_animator.play() << "fire"
                                  << thor::Playback::notify([this]{
                    m_animPlaying = false;
                });
            }
        }
    }
    else
    {
        if (m_res)
        {
            playSoundUI(*m_res, "weaponclick");
        }
    }
}

void Weapon::update(const sf::Time &t_deltaTime)
{
    if (m_idleAnimSet)
    {
        if (!m_animPlaying)
        {
            m_animator.play() << thor::Playback::loop("idle");
            setOrigin(m_spriteSheet.getTextureRect().width / 2.f, m_spriteSheet.getTextureRect().height);
        }
    }
    m_animator.update(t_deltaTime);
    m_animator.animate(m_spriteSheet);
}

void Weapon::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_spriteSheet, t_states);
}

void Weapon::reloadMagazine()
{
    if (m_magazineSize >= 0)
    {
        if (m_ammoPool >= m_magazineSize)
        {
            m_ammoInMagazine = m_magazineSize;
            m_ammoPool -= m_magazineSize;
        }
        else
        {
            m_ammoInMagazine = m_ammoPool;
            m_ammoPool = 0;
        }
    }
}

void Weapon::fireAmmo()
{
    if (m_magazineSize >= 0)
    {
        if (m_ammoInMagazine > 0)
        {
            --m_ammoInMagazine;
        }
        else
        {

        }
    }
    else if (m_ammoPool > 0)
    {
        --m_ammoPool;
    }
}

bool Weapon::checkCanFire()
{
    if (m_magazineSize >= 0)
    {
        return m_ammoInMagazine > 0;
    }
    else
    {
        return m_ammoPool > 0;
    }
}

bool Weapon::checkCanReload()
{
    if (m_magazineSize >= 0)
    {
        return m_ammoPool > 0;
    }
    else
    {
        return false;
    }
}

}
