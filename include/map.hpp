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

#include <memory>
#include <vector>

#include <boost/optional.hpp>

#include <SFML/System/Vector2.hpp>

#include "common.hpp"

#include "tile.hpp"
#include "trigger.hpp"
#include "sector.hpp"
#include "drawableactor.hpp"

namespace Rayfun
{

class Map : sf::NonCopyable
{
    public:
        Map() = default;
        explicit Map(const sf::Vector2s& t_size);

    public:
        void setTileAt(const sf::Vector2s &t_pos, const Tile& t_tile);
        const Tile &tileAt(const sf::Vector2s &t_pos) const;
        Tile &tileAt(const sf::Vector2s &t_pos);

        void setSize(const sf::Vector2s& t_size);
        sf::Vector2s size() const;

        boost::optional<Sector&> sectorAt(const sf::Vector2s &t_pos);
        boost::optional<Sector> sectorAt(const sf::Vector2s &t_pos) const;

        void update(const sf::Time& t_deltaTime);

        const Tile* tiles() const
        { return m_tiles.data(); }

    public:
        std::vector<Sector> sectors {};
        std::vector<Trigger> triggers {};
        mutable std::vector<std::unique_ptr<DrawableActor>> sprites;
        unsigned char ambientLight { 200 };

        sf::Vector2d playerPos { 0, 0 };

    private:
        //boost::multi_array<Tile, 2> m_tiles {};
        std::vector<Tile> m_tiles;
        sf::Vector2s m_size;
};

} // namespace Rayfun

#endif // MAP_HPP
