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

namespace Rayfun
{

Map::Map(const sf::Vector2s &t_size)
    : m_tiles(boost::extents[long(t_size.x)][long(t_size.y)])
{

}

Map::Map(boost::const_multi_array_ref<Tile, 2> t_tiles)
    : m_tiles(t_tiles)
{

}

void Map::setTileAt(const sf::Vector2s &t_pos, const Tile &t_tile)
{
    assert(t_pos.x < m_tiles.shape()[0] && t_pos.y < m_tiles.shape()[1]);
    m_tiles[long(t_pos.x)][long(t_pos.y)] = t_tile;
}

Tile Map::tileAt(const sf::Vector2s &t_pos) const
{
    assert(t_pos.x < m_tiles.shape()[0] && t_pos.y < m_tiles.shape()[1]);
    return m_tiles[long(t_pos.x)][long(t_pos.y)];
}

sf::Vector2s Map::size() const
{
    return sf::Vector2s(m_tiles.shape()[0], m_tiles.shape()[1]);
}

} // namespace Rayfun
