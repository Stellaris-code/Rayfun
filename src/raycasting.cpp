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

#include "raycasting.hpp"

#include <iostream>

#include <cmath>

#include <Thor/Vectors.hpp>

#include "camera.hpp"
#include "map.hpp"

#include "utility.hpp"

namespace Rayfun
{

namespace Raycasting
{

RaycastResult castRay(const sf::Vector2d &t_begin, const sf::Vector2d &t_dir, const Map& t_map, HitMode t_hitmode)
{
    const sf::Vector2d rayDir = t_dir;
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
        auto correctedDir = thor::rotatedVector(thor::unitVector(t_dir), -90.0);
        if (sideDist.x < sideDist.y) // hit on x size
        {
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            side = correctedDir.y > 0 ? Side::South : Side::North;
        }
        else // hit on y side
        {
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            side = correctedDir.x > 0 ? Side::East : Side::West;
        }

        if (mapPos.x < 0 || mapPos.y < 0 ||
                static_cast<unsigned>(mapPos.x) >= t_map.size().x ||
                static_cast<unsigned>(mapPos.y) >= t_map.size().y) // Outside of map bounds
        {
            return RaycastResult{ side };
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
        perpWallDist = (mapPos.x - t_begin.x + (1 - step.x) / 2.f) / rayDir.x;
    }
    else
    {
        perpWallDist = (mapPos.y - t_begin.y + (1 - step.y) / 2.f) / rayDir.y;
    }
    if (!std::isfinite(perpWallDist))
    {
        perpWallDist = 0;
    }

    double wallX; //where exactly the wall was hit
    if (side == Side::North || side == Side::South)
    {
        wallX = t_begin.y + perpWallDist * rayDir.y;
        sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(0, wallX);

        return { side, t_map.tileAt(sf::Vector2s(mapPos)), hitPos, mapPos,
                    Utility::distance(t_begin, hitPos), perpWallDist };
    }
    else
    {
        wallX = t_begin.x + perpWallDist * rayDir.x;
        sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(wallX, 0);

        return { side, t_map.tileAt(sf::Vector2s(mapPos)), hitPos, mapPos,
                    Utility::distance(t_begin, hitPos), perpWallDist };
    }
}

std::vector<sf::Uint8> render(const Camera& t_cam, const Map& t_map)
{
    std::vector<sf::Uint8> renderTarget;
    renderTarget.resize(t_cam.screenSize().x * t_cam.screenSize().y * 4);
    std::fill(renderTarget.begin(), renderTarget.end(), 0);
    for (size_t i { 0 }; i < t_cam.screenSize().x; ++i)
    {
        double cameraX = 2.f * i / t_cam.screenSize().x - 1;
        sf::Vector2d dir = t_cam.direction() + t_cam.plane() * cameraX;
        auto result = castRay(t_cam.pos(), dir, t_map, HitMode::Visibility);

        long lineHeight = static_cast<long>(t_cam.screenSize().y / result.perpDistance);
        long drawStart = -lineHeight / 2 + static_cast<long>(t_cam.screenSize().y) / 2;
        long drawStartClamped = drawStart;
        if (drawStart < 0)
        {
            drawStartClamped = 0;
        }
        long drawEnd = static_cast<long>(lineHeight / 2 +
                                         static_cast<long>(t_cam.screenSize().y) / 2);
        long drawEndClamped = drawEnd;
        if (drawEnd >= static_cast<long>(t_cam.screenSize().y))
        {
            drawEndClamped = static_cast<long>(t_cam.screenSize().y) - 1;
        }

        if (result.tileHit)
        {
            double wallX { 0 };
            if (result.side == Side::North || result.side == Side::South)
            {
                wallX = result.hitPos.y - std::floor(result.hitPos.y);
            }
            else
            {
                wallX = result.hitPos.x - std::floor(result.hitPos.x);
            }

            unsigned texX = static_cast<unsigned>(wallX * static_cast<double>(
                                                      result.tileHit->tex[result.side].getSize().x));

            if (((result.side == Side::North || result.side == Side::South) && dir.x > 0) ||
                    ((result.side == Side::East || result.side == Side::West) && dir.y < 0))
            {
                texX = result.tileHit->tex[result.side].getSize().x - texX - 1;
            }

            for (size_t j { static_cast<size_t>(drawStartClamped) }; static_cast<long>(j) < drawEndClamped; ++j)
            {
                unsigned texY = Utility::map<unsigned>(static_cast<unsigned>(j),
                                                       static_cast<unsigned>(drawStart),
                                                       static_cast<unsigned>(drawEnd), 0,
                                                       result.tileHit->tex[result.side].getSize().y);

                sf::Color color = result.tileHit->tex[result.side].getPixel(texX, texY);
                renderTarget[(t_cam.screenSize().x * j + i) * 4 + 0] = color.r;
                renderTarget[(t_cam.screenSize().x * j + i) * 4 + 1] = color.g;
                renderTarget[(t_cam.screenSize().x * j + i) * 4 + 2] = color.b;
                renderTarget[(t_cam.screenSize().x * j + i) * 4 + 3] = 0xFF;
            }

            sf::Vector2d floorWall {};
            switch (result.side)
            {
                case Side::North:
                    floorWall.x = result.mapPos.x;
                    floorWall.y = result.mapPos.y + wallX;
                    break;

                case Side::South:
                    floorWall.x = result.mapPos.x + 1;
                    floorWall.y = result.mapPos.y + wallX;
                    break;

                case Side::East:
                    floorWall.x = result.mapPos.x + wallX;
                    floorWall.y = result.mapPos.y;
                    break;

                case Side::West:
                    floorWall.x = result.mapPos.x + wallX;
                    floorWall.y = result.mapPos.y + 1;
                    break;
            }

            if (drawEndClamped < 0)
            {
                drawEndClamped = static_cast<long>(t_cam.screenSize().y);
            }

            for (size_t j { static_cast<size_t>(drawEndClamped) + 1 }; j < t_cam.screenSize().y; ++j)
            {
                double currentDist = t_cam.screenSize().y / (2.f * j - t_cam.screenSize().y);
                double weight = currentDist / result.perpDistance;
                sf::Vector2d floorPos = weight * floorWall +
                                        (1.0 - weight) * t_cam.pos();

                auto sector = t_map.sectorAt(sf::Vector2s(floorPos));

                if (sector)
                {
                    sf::Vector2u floorTexCoord {};
                    floorTexCoord.x = static_cast<unsigned>(floorPos.x * sector.get().floor.getSize().x)
                                      % sector.get().floor.getSize().x;
                    floorTexCoord.y = static_cast<unsigned>(floorPos.y * sector.get().floor.getSize().y)
                                      % sector.get().floor.getSize().y;

                    sf::Vector2u ceilTexCoord {};
                    ceilTexCoord.x = static_cast<unsigned>(floorPos.x * sector.get().ceiling.getSize().x)
                                     % sector.get().ceiling.getSize().x;
                    ceilTexCoord.y = static_cast<unsigned>(floorPos.y * sector.get().ceiling.getSize().y)
                                     % sector.get().ceiling.getSize().y;

                    sf::Color floorTexel = sector.get().floor.getPixel(floorTexCoord.x, floorTexCoord.y);
                    sf::Color ceilTexel = sector.get().ceiling.getPixel(ceilTexCoord.x, ceilTexCoord.y);

                    renderTarget[(t_cam.screenSize().x * (j-1) + i) * 4 + 0] = floorTexel.r;
                    renderTarget[(t_cam.screenSize().x * (j-1) + i) * 4 + 1] = floorTexel.g;
                    renderTarget[(t_cam.screenSize().x * (j-1) + i) * 4 + 2] = floorTexel.b;
                    renderTarget[(t_cam.screenSize().x * (j-1) + i) * 4 + 3] = 0xFF;

                    renderTarget[(t_cam.screenSize().x * (t_cam.screenSize().y - j) + i) * 4 + 0] = ceilTexel.r;
                    renderTarget[(t_cam.screenSize().x * (t_cam.screenSize().y - j) + i) * 4 + 1] = ceilTexel.g;
                    renderTarget[(t_cam.screenSize().x * (t_cam.screenSize().y - j) + i) * 4 + 2] = ceilTexel.b;
                    renderTarget[(t_cam.screenSize().x * (t_cam.screenSize().y - j) + i) * 4 + 3] = 0xFF;
                }
            }
        }
    }
    return renderTarget;
}

} // namespace Raycasting

} // namespace Rayfun
