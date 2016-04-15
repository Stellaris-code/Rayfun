/*  include/map.hpp Map - Yann BOUCHER (yann) 12/04/2016
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
#ifndef MAP_HPP
#define MAP_HPP

#include <cassert>

#include <vector>

#include <boost/multi_array.hpp>
#include <boost/optional.hpp>

#include <SFML/System/Vector2.hpp>

#include "common.hpp"

#include "tile.hpp"
#include "sector.hpp"

namespace Rayfun
{

class Map
{
    public:
        explicit Map(const sf::Vector2s& t_size);
        explicit Map(boost::const_multi_array_ref<Tile, 2> t_tiles);

    public:
        void setTileAt(const sf::Vector2s &t_pos, const Tile& t_tile);
        Tile tileAt(const sf::Vector2s &t_pos) const;

        sf::Vector2s size() const;

        boost::optional<Sector&> sectorAt(const sf::Vector2s &t_pos);
        boost::optional<Sector> sectorAt(const sf::Vector2s &t_pos) const;

    public:
        std::vector<Sector> sectors {};

    private:
        boost::multi_array<Tile, 2> m_tiles {};
};

} // namespace Rayfun

#endif // MAP_HPP
