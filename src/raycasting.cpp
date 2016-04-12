/*  raycasting %{Cpp:License:ClassName} - Yann BOUCHER (yann) 12/04/2016
 *
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

#include <iostream> // FIXME remove for release

#include <cmath>

#include <Thor/Vectors.hpp>

#include "raycasting.hpp"

#include "camera.hpp"
#include "map.hpp"

#include "utility.hpp"

namespace Rayfun
{

namespace Raycasting
{

RaycastResult castRay(const sf::Vector2d &t_begin, const sf::Vector2d &t_dir, const Map& t_map, HitMode t_hitmode)
{
    const sf::Vector2d rayDir = thor::unitVector(t_dir);
    auto mapPos = sf::Vector2i(t_begin);

    const sf::Vector2d deltaDist = {
        std::sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
        std::sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
    };

    sf::Vector2d sideDist;
    sf::Vector2i step;

    if (rayDir.x < 0)
    {
        step.x = -1;
        sideDist.x = (t_begin.x - mapPos.x) * deltaDist.x;
    }
    else
    {
        step.x = 1;
        sideDist.x = (mapPos.x + 1 - t_begin.x) * deltaDist.x;
    }
    if (rayDir.y < 0)
    {
        step.y = -1;
        sideDist.y = (t_begin.y - mapPos.y) * deltaDist.y;
    }
    else
    {
        step.y = 1;
        sideDist.y = (mapPos.y + 1 - t_begin.y) * deltaDist.y;
    }

    Side side { Side::North };
    bool hit { false };

    do
    {
        if (sideDist.x < sideDist.y) // hit on x size
        {
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            //side = t_begin.y > t_end.y ? Side::South : Side::North;
        }
        else // hit on y side
        {
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            //side = t_begin.x > t_end.x ? Side::East : Side::West;
        }

        if (mapPos.x < 0 || mapPos.y < 0 ||
                static_cast<unsigned>(mapPos.x) > t_map.size().x ||
                static_cast<unsigned>(mapPos.y) > t_map.size().y) // Outside of map bounds
        {
            return RaycastResult{};
        }
        else
        {
            switch (t_hitmode)
            {
                case HitMode::Visibility:
                    hit = t_map.tileAt(sf::Vector2s(mapPos)).isWall;
                    break;
                case HitMode::Clipping:
                    hit = t_map.tileAt(sf::Vector2s(mapPos)).clip[side];
                    break;

            }
        }
    } while (!hit);

    double perpWallDist { 0 };

    if (side == Side::North || side == Side::South)
    {
        if (rayDir.x == 0)
        {
            perpWallDist = 0;
        }
        else
        {
            perpWallDist = (mapPos.x - t_begin.x + (1 - step.x) / 2) / rayDir.x;
        }
    }
    else
    {
        if (rayDir.y == 0)
        {
            perpWallDist = 0;
        }
        else
        {
            perpWallDist = (mapPos.y - t_begin.x + (1 - step.y) / 2) / rayDir.y;
        }
    }

    double wallX; //where exactly the wall was hit
    if (side == Side::North || side == Side::South)
    {
        wallX = t_begin.y + perpWallDist * rayDir.y;
        sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(0, wallX);

        return { t_map.tileAt(sf::Vector2s(mapPos)), hitPos,
                    Utility::distance(t_begin, hitPos), perpWallDist };
    }
    else
    {
        wallX = t_begin.x + perpWallDist * rayDir.x;
        sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(wallX, 0);

        return { t_map.tileAt(sf::Vector2s(mapPos)), hitPos,
                    Utility::distance(t_begin, hitPos), perpWallDist };
    }
}

} // namespace Raycasting

} // namespace Rayfun
