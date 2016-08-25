/*  src/map.cpp Map - Yann BOUCHER (yann) 12/04/2016
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
#include "map.hpp"

#include "utils/graphicsutility.hpp"

namespace Rayfun
{

Map::Map(const sf::Vector2s &t_size)
    : m_tiles(t_size.x * t_size.y), m_size(t_size)
{

}

void Map::setTileAt(const sf::Vector2s &t_pos, const Tile &t_tile)
{
    assert(t_pos.x < m_size.x && t_pos.y < m_size.y);
    m_tiles[t_pos.y * m_size.x + t_pos.x] = t_tile;
}

const Tile& Map::tileAt(const sf::Vector2s &t_pos) const
{
    assert(t_pos.x < m_size.x && t_pos.y < m_size.y);
    return m_tiles[t_pos.y * m_size.x + t_pos.x];
}
Tile &Map::tileAt(const sf::Vector2s &t_pos)
{
    assert(t_pos.x < m_size.x && t_pos.y < m_size.y);
    return m_tiles[t_pos.y * m_size.x + t_pos.x];
}

void Map::setSize(const sf::Vector2s &t_size)
{
    m_size = t_size;
    m_tiles.resize(t_size.x * t_size.y);
}

sf::Vector2s Map::size() const
{
    return m_size;
}

boost::optional<Sector&> Map::sectorAt(const sf::Vector2s& t_pos)
{
    for (auto& s : sectors)
    {
        if (s.rect.contains(t_pos))
        {
            return s;
        }
    }

    return boost::none;
}

boost::optional<Sector> Map::sectorAt(const sf::Vector2s& t_pos) const
{
    for (const auto& s : sectors)
    {
        if (s.rect.contains(t_pos))
        {
            return s;
        }
    }

    return boost::none;
}

void Rayfun::Map::update(const sf::Time &t_deltaTime)
{
    for (size_t i { 0 }; i < m_size.x; ++i)
    {
        for (size_t j { 0 }; j < m_size.y; ++j)
        {
            auto& tile = m_tiles[i * m_size.x + j];
            if (tile.sliding)
            {
                tile.slideProgress += t_deltaTime;
//                Utility::slideImage(*tile.tex[tile.slideSide], tile.slideDirection,
//                        (t_deltaTime / tile.slideSpeed) * (tile.slideDirection == Side::North || tile.slideDirection == Side::South ? tile.tex[tile.slideSide]->getSize().y
//                                                           : tile.tex[tile.slideSide]->getSize().x));
                if (tile.decals[tile.slideSide])
                {
                    Utility::slideImage(*tile.decals[tile.slideSide], tile.slideDirection,
                            (t_deltaTime / tile.slideSpeed) * (tile.slideDirection == Side::North || tile.slideDirection == Side::South ? tile.decals[tile.slideSide]->getSize().y
                                                               : tile.decals[tile.slideSide]->getSize().x));
                }
                if (tile.slideProgress >= tile.slideSpeed)
                {
                    tile.sliding = false;
                    tile.clip[tile.slideSide] = false;
                }
            }
        }
    }

    for (auto& sprite : sprites)
    {
        sprite->playerPos = playerPos;
        sprite->update(t_deltaTime);
    }
}

} // namespace Rayfun
