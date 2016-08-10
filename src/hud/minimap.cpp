/*  src/hud/minimap.cpp Minimap - Yann BOUCHER (yann) 18/05/2016
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
#include "hud/minimap.hpp"

#include <cassert>

#include "graphicsutility.hpp"

#include "map.hpp"

namespace Rayfun
{

Minimap::Minimap(const Map &t_map, const Resources &t_res, const sf::Vector2f& t_size)
    : m_map(&t_map)
{
    setMap(t_map);
    m_tilemap.setTexture(t_res.textureHolder["hud/minimap/tileset"]);
    m_tilemap.setNumberOfTextureTilesPerRow(2);
    m_tilemap.setTextureTileSize({ 8, 8 });
    m_tilemap.setOutOfBoundsTile(0);
    m_tilemap.setSize(t_size);
    m_tilemap.setColor(sf::Color(255, 255, 255, 200));

    m_arrow.setTexture(t_res.textureHolder["hud/minimap/arrow"]);
    m_arrow.setScale(m_tilemap.getTextureTileSize().x / m_arrow.getLocalBounds().width,
                     m_tilemap.getTextureTileSize().y / m_arrow.getLocalBounds().height);

    Utility::setOriginToCenter(m_arrow);
    //m_tilemap.setPosition(t_size / 2.f);


    assert(m_map != nullptr);

    m_tilemap.setGridSize(sf::Vector2u(m_map->size()));
}

void Minimap::setMap(const Map &t_map)
{
    m_map = &t_map;
    m_tilemap.setGridSize(sf::Vector2u(m_map->size()));
    updateTilemap();
    assert(m_map != nullptr);
}

void Minimap::update(sf::Time)
{
    updateTilemap();
    m_arrow.setRotation(playerAngle);
}

void Minimap::updateTilemap()
{
    assert(m_map != nullptr);

    std::vector<unsigned> indexMap;
    indexMap.resize(m_map->size().x * m_map->size().y);
    for (size_t i { 0 }; i < m_map->size().x; ++i)
    {
        for (size_t j { 0 }; j < m_map->size().y; ++j)
        {
            indexMap[j * m_map->size().x + i] = m_map->tileAt({i, j}).isWall ? 1 : 0;
        }
    }
    m_arrow.setPosition(playerPos.x * (m_tilemap.getSize().x / m_tilemap.getGridSize().x),
                        playerPos.y * (m_tilemap.getSize().y / m_tilemap.getGridSize().y));
    //m_tilemap.setCameraTargetTile(playerPos);
    m_tilemap.update(indexMap, m_map->size().x);
}

void Minimap::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_tilemap, t_states);
    t_target.draw(m_arrow, t_states);
}

}
