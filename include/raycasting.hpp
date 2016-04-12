/*  raycasting %{Cpp:License:ClassName} - Yann BOUCHER (yann) 12/04/2016
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
#ifndef RAYCASTING_HPP
#define RAYCASTING_HPP

#include <boost/optional.hpp>

#include "common.hpp"

#include "tile.hpp"

namespace Rayfun
{

class Camera;
class Map;

namespace Raycasting
{

struct RaycastResult
{
        const boost::optional<Tile> tileHit {};
        const sf::Vector2d hitPos {};
        const double distance { 0 };
        const double perpDistance { 0 };
};

enum class HitMode
{
    Visibility,
    Clipping
};

RaycastResult castRay(const sf::Vector2d& t_begin, const sf::Vector2d& t_dir, const Map& t_map,
                      HitMode t_hitmode = HitMode::Visibility);

}

}

#endif // RAYCASTING_HPP
