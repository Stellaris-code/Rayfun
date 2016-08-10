/*  include/hud.hpp HUD - Yann BOUCHER (yann) 20/06/2016
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
#ifndef HUD_HPP
#define HUD_HPP

#include <map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Text.hpp>

#include <SFML/System/Time.hpp>

#include "common.hpp"

namespace Rayfun
{

class Weapon;
class Player;

class HUD : public sf::Drawable, public sf::Transformable
{
    public:
        struct PlayerInfo
        {
                unsigned health { 100 };
                unsigned armor { 0 };
                std::map<std::string, unsigned long> ammo;
        };

    public:
        explicit HUD(const Resources& t_res);
        virtual ~HUD() = default;

    public:
        void setWeapon(Weapon *t_weapon);
        void setSize(const sf::Vector2d& t_size);
        void setPlayerInfo(const PlayerInfo& t_plyInfo);

        virtual void update(sf::Time t_deltaTime);

        sf::Vector2d size() const
        { return m_size; }

    public:
        double moveSpeed { 0 };

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        void updatePos();
        void updateBob(double t_amount);

    private:
        Weapon* m_weapon { nullptr };
        const Resources& m_res;
        sf::Vector2d m_size { 0, 0 };
        sf::Text m_lifeArmor;
        sf::Text m_ammoList;
        double m_runningTime;
};

}

#endif // HUD_HPP
