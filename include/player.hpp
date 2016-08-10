/*  include/player.hpp Player - Yann BOUCHER (yann) 08/05/2016
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
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <vector>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

#include "camera.hpp"
#include "actor.hpp"

namespace Rayfun
{

class Weapon;
struct WeaponStructure;

class Player : public Actor
{
        using Actor::Actor;

    public:
        virtual void update(const sf::Time&) override;

    public:
        void firePrimary();
        void fireSecondary();
        void reload();
        void move(double t_amount);
        void strafe(double t_amount);
        void turn(double t_amount);
        void use();

        unsigned long & ammo(const std::string& t_type);
        unsigned long ammo(const std::string& t_type) const;
        std::map<std::string, unsigned long> ammo_map() const;

        double movementAmount() const;

        void addWeapon(const WeaponStructure& t_wep);

    public:
        unsigned armor { 0 };
        unsigned health { 100 };
        std::vector<Weapon> weapons;

    private:
        mutable std::map<std::string, unsigned long> m_ammo;
        sf::Vector2d m_deltaPos;

    private:
};

} // namespace Rayfun

#endif // PLAYER_HPP
