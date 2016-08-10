/*  include/hud/minimap.hpp Minimap - Yann BOUCHER (yann) 18/05/2016
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
#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include <SelbaWard/TileMap.hpp>

#include "common.hpp"

namespace Rayfun
{

class Map;

class Minimap : public sf::Drawable, public sf::Transformable
{
    public:
        Minimap(const Map& t_map, const Resources& t_res, const sf::Vector2f& t_size);

    public:
        void setMap(const Map& t_map);

        void update(sf::Time t_delaTime);

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        void updateTilemap();

    public:
        sf::Vector2f playerPos;
        float playerAngle { 0 };

    private:
        const Map* m_map { nullptr };
        selbaward::TileMap m_tilemap;
        sf::Sprite m_arrow;
};

}

#endif // MINIMAP_HPP
