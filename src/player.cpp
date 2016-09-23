/*  src/player.cpp Player - Yann BOUCHER (yann) 08/05/2016
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
#include "player.hpp"

#include <SFML/Audio/Listener.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "pakcontents.hpp"

#include "utils/gameplayglobals.hpp"
#include "utils/mathutility.hpp"
#include "raycasting.hpp"

#include "weapon.hpp"
#include "weaponstructure.hpp"

namespace Rayfun
{

void Player::update(const sf::Time& t_deltaTime)
{
    sf::Listener::setPosition(pos.x, pos.y, 0);
    sf::Listener::setDirection(Utility::angleToVector(angle).x, Utility::angleToVector(angle).y, 0);
    for (auto& wep : weapons)
    {
        wep.update(t_deltaTime);
    }

    if (!m_moved)
    {
        m_deltaPos = { 0, 0 };
    }
    else
    {
        m_moved = false;
    }
}

void Player::firePrimary()
{
    weapons[0].fire();
}

void Player::fireSecondary()
{

}

void Player::reload()
{

}

void Player::move(double t_amount)
{
    auto newPos = pos + thor::rotatedVector(sf::Vector2d(t_amount, 0), angle);
    auto result = Raycasting::castRay(pos, thor::rotatedVector(sf::Vector2d(t_amount, 0), angle),
                                      m_context.mapPack.level.map, Raycasting::HitMode::Clipping);

    DrawableActor* act = nullptr;

    if (!result.front().tileHit || ((sf::Vector2s(pos) == sf::Vector2s(newPos) ||
         (!result.front().tileHit->clip[result.front().side] ||
          !m_context.mapPack.level.map.tileAt(sf::Vector2s(newPos)).clip[result.front().side]) ||
         !m_context.mapPack.level.map.tileAt(sf::Vector2s(newPos)).isWall) &&
            !spriteClipAt(newPos, m_context.mapPack.level.map, act)))
    {
        m_deltaPos = newPos - pos;
        pos = newPos;
        m_moved = true;
    }
}

void Player::strafe(double t_amount)
{
    auto newPos = pos + thor::rotatedVector(sf::Vector2d(t_amount, 0), angle + 90);
    auto result = Raycasting::castRay(pos, thor::rotatedVector(sf::Vector2d(t_amount, 0), angle + 90),
                                      m_context.mapPack.level.map, Raycasting::HitMode::Clipping);

    DrawableActor* act = nullptr;

    if (!result.front().tileHit || ((sf::Vector2s(pos) == sf::Vector2s(newPos) ||
         (!result.front().tileHit->clip[result.front().side] ||
          !m_context.mapPack.level.map.tileAt(sf::Vector2s(newPos)).clip[result.front().side]) ||
         !m_context.mapPack.level.map.tileAt(sf::Vector2s(newPos)).isWall) &&
            !spriteClipAt(newPos, m_context.mapPack.level.map, act)))
    {
        m_deltaPos = newPos - pos;
        pos = newPos;
        m_moved = true;
    }
}

void Player::turn(double t_amount)
{
    angle += t_amount;
}

void Player::use()
{
    auto result = Raycasting::castRay(pos, Utility::angleToVector(angle),
                                      m_context.mapPack.level.map, Raycasting::HitMode::Visibility).front();
    DrawableActor* actor = nullptr;
    if (result.distance <= 1 && result.tileHit)
    {
        if (result.tileHit->ontrigger[result.side])
        {
            result.tileHit->ontrigger[result.side](*this, *result.tileHit);
        }
    }
    else if (Raycasting::rayIntersectsSprite(pos, Utility::angleToVector(angle),
                                             m_context.mapPack.level.map,
                                             actor, 1))
    {
        if (actor && actor->ontrigger)
        {
            actor->ontrigger(*this, *actor);
        }
    }
}

unsigned long &Player::ammo(const std::string &t_type)
{
    if (m_ammo.count(t_type) == 0)
    {
        m_ammo[t_type] = 0;
    }
    return m_ammo.at(t_type);
}

unsigned long Player::ammo(const std::string &t_type) const
{
    if (m_ammo.count(t_type) == 0)
    {
        m_ammo[t_type] = 0;
    }
    return m_ammo.at(t_type);
}

std::map<std::string, unsigned long> Player::ammo_map() const
{
    return m_ammo;
}

double Player::movementAmount() const
{
    return thor::length(m_deltaPos);
}

void Player::addWeapon(const WeaponStructure &t_wep)
{
    weapons.emplace_back(ammo(t_wep.ammoType), t_wep.magazineSize);
    auto& weapon = weapons.back();
    weapon.name = t_wep.name;
    weapon.setSpriteSheet(t_wep.spriteSheet);
    if (t_wep.fireAnimDuration > sf::Time::Zero)
    {
        weapon.setFireAnimation(t_wep.fire_anim, t_wep.fireAnimDuration);
    }
    if (t_wep.reloadAnimDuration > sf::Time::Zero)
    {
        weapon.setReloadAnimation(t_wep.reload_anim, t_wep.reloadAnimDuration);
    }
    if (t_wep.idleAnimDuration > sf::Time::Zero)
    {
        weapon.setIdleAnimation(t_wep.idle_anim, t_wep.idleAnimDuration);
    }

    weapon.setFireCallback(t_wep.fire_callback);
    weapon.setReloadCallback(t_wep.reload_callback);
}

} // namespace Rayfun
