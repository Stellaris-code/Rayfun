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

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <algorithm>
#include <iostream>

#include <cmath>

#include <Thor/Vectors.hpp>
#include <Thor/Graphics/ColorGradient.hpp>

#include "camera.hpp"
#include "map.hpp"
#include "drawableactor.hpp"

#include "mathutility.hpp"
#include "graphicsutility.hpp"
#include "utility.hpp"

namespace Rayfun
{

namespace Raycasting
{

std::vector<RaycastResult> castRay(const sf::Vector2d &t_begin, const sf::Vector2d &t_dir, Map& t_map, HitMode t_hitmode)
{
    const sf::Vector2d rayDir = thor::unitVector(t_dir);
    auto mapPos = sf::Vector2i(t_begin);

    const sf::Vector2d deltaDist = {
        std::sqrt(1 + (t_dir.y * t_dir.y) / (t_dir.x * t_dir.x)),
        std::sqrt(1 + (t_dir.x * t_dir.x) / (t_dir.y * t_dir.y))
    };

    sf::Vector2d sideDist;
    const sf::Vector2i step = { rayDir.x < 0 ? -1 : 1, rayDir.y < 0 ? -1 : 1 };

    if (rayDir.x < 0)
    {
        sideDist.x = (t_begin.x - mapPos.x) * deltaDist.x;
    }
    else
    {
        sideDist.x = (mapPos.x + 1 - t_begin.x) * deltaDist.x;
    }
    if (rayDir.y < 0)
    {
        sideDist.y = (t_begin.y - mapPos.y) * deltaDist.y;
    }
    else
    {
        sideDist.y = (mapPos.y + 1 - t_begin.y) * deltaDist.y;
    }

    Side side { Side::North };
    bool hit { false };
    std::vector<RaycastResult> results;
    const auto correctedDir = thor::rotatedVector(rayDir, -90.0);

    while (true)
    {

        do
        {
            if (sideDist.x < sideDist.y) // hit on x size
            {
                sideDist.x += deltaDist.x;
                mapPos.x += step.x;
                side = correctedDir.y > 0 ? Side::North : Side::South;
            }
            else // hit on y side
            {
                sideDist.y += deltaDist.y;
                mapPos.y += step.y;
                side = correctedDir.x > 0 ? Side::East : Side::West;
            }

            if (BOOST_UNLIKELY(mapPos.x < 0) || BOOST_UNLIKELY(mapPos.y < 0) ||
                    BOOST_UNLIKELY(static_cast<unsigned>(mapPos.x) >= t_map.size().x) ||
                    BOOST_UNLIKELY(static_cast<unsigned>(mapPos.y) >= t_map.size().y)) // Outside of map bounds
            {
                if (results.empty())
                {
                    results.push_back({ side });
                }
                return results;
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
            perpWallDist = (mapPos.x - t_begin.x + (1 - step.x) / 2.f) / t_dir.x;
        }
        else
        {
            perpWallDist = (mapPos.y - t_begin.y + (1 - step.y) / 2.f) / t_dir.y;
        }
        if (!std::isfinite(perpWallDist))
        {
            perpWallDist = 0;
        }

        //mapPos = thor::rotated(sf::Vector2d(mapPos), 90);

        auto& tileHit = t_map.tileAt(sf::Vector2s(mapPos));
        if (tileHit.tex[side])
        {
            if (side == Side::North || side == Side::South)
            {

                double wallX { }; //where exactly the wall was hit
                wallX = t_begin.y + perpWallDist * t_dir.y;
                wallX -= std::floor(wallX);
                wallX = 1 - wallX;

                unsigned texX = static_cast<unsigned>(wallX * static_cast<double>(
                                                          tileHit.tex[side]->getSize().x));

                if (t_dir.x > 0)
                {
                    if (static_cast<int>(tileHit.tex[side]->getSize().x - texX - 1) < 0)
                    {
                        texX = 0;
                    }
                    else
                    {
                        texX = tileHit.tex[side]->getSize().x - texX - 1;
                    }
                }

                sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(0, wallX);

                results.push_back({ side, tileHit, hitPos, mapPos,
                                    Utility::distance(t_begin, hitPos), perpWallDist });

                if (t_hitmode == HitMode::Clipping || !Utility::imageStripContainsAlpha(*tileHit.tex[side], texX))
                {
                    return results;
                }
            }
            else
            {

                double wallX { }; //where exactly the wall was hit
                wallX = t_begin.x + perpWallDist * t_dir.x;
                wallX -= std::floor(wallX);
                wallX = 1 - wallX;

                unsigned texX = static_cast<unsigned>(wallX * static_cast<double>(
                                                          tileHit.tex[side]->getSize().x));

                if (t_dir.y < 0)
                {
                    if (static_cast<int>(tileHit.tex[side]->getSize().x - texX - 1) < 0)
                    {
                        texX = 0;
                    }
                    else
                    {
                        texX = tileHit.tex[side]->getSize().x - texX - 1;
                    }
                }

                sf::Vector2d hitPos = sf::Vector2d(mapPos) + sf::Vector2d(wallX, 0);

                results.push_back({ side, tileHit, hitPos, mapPos,
                                    Utility::distance(t_begin, hitPos), perpWallDist });

                if (t_hitmode == HitMode::Clipping || !Utility::imageStripContainsAlpha(*tileHit.tex[side], texX))
                {
                    return results;
                }
            }

        }

    }

}

std::vector<sf::Uint8> render(const Camera& t_cam, const Map& t_map, bool bilinear_filtering,
                              bool t_bilinear_sprites)
{
    std::vector<sf::Uint8> renderTarget;
    renderTarget.resize(t_cam.screenSize().x * t_cam.screenSize().y * 4);
    std::fill(renderTarget.begin(), renderTarget.end(), 0);

    std::vector<double> zbuffer;
    zbuffer.resize(t_cam.screenSize().x);

    std::sort(t_map.sprites.begin(), t_map.sprites.end(), [&t_cam](std::unique_ptr<DrawableActor>& rhs,
              std::unique_ptr<DrawableActor>& lhs){
        return Utility::distance(rhs->pos, t_cam.pos()) > Utility::distance(lhs->pos, t_cam.pos());
    });

    static Utility::LookupTable<double> lut(t_cam.screenSize().x, [&t_cam](size_t i){
        return 2.f * i / t_cam.screenSize().x - 1;
    });

#pragma omp parallel for simd
    for (size_t i = 0; i < t_cam.screenSize().x; ++i)
    {
        double cameraX = lut[i];
        //double cameraX = 2.f * i / t_cam.screenSize().x - 1;
        const sf::Vector2d dir = t_cam.direction() + t_cam.plane() * cameraX;
        const auto& results = castRay(t_cam.pos(), dir, const_cast<Map&>(t_map), HitMode::Visibility);

        for (size_t index { results.size() }; index > 0; --index)
        {
            const auto result = results[index - 1];
            const long lineHeight = static_cast<long>(t_cam.screenSize().y / result.perpDistance) *
                                    std::min<double>(t_cam.screenSize().x, t_cam.screenSize().y)
                                    / std::max<double>(t_cam.screenSize().x, t_cam.screenSize().y);
            const long drawStart = -lineHeight / 2 + static_cast<long>(t_cam.screenSize().y) / 2;

            if (index == results.size())
            {
                zbuffer[i] = result.perpDistance;
            }

            long drawStartClamped = drawStart;
            if (drawStart < 0)
            {
                drawStartClamped = 0;
            }
            const long drawEnd = static_cast<long>(lineHeight / 2 +
                                                   static_cast<long>(t_cam.screenSize().y) / 2);
            long drawEndClamped = drawEnd;
            if (drawEnd >= static_cast<long>(t_cam.screenSize().y))
            {
                drawEndClamped = static_cast<long>(t_cam.screenSize().y) - 1;
            }

            if (BOOST_LIKELY((bool)result.tileHit) && BOOST_LIKELY(result.tileHit->tex[result.side] != nullptr))
            {
                const double wallX = { result.side == Side::North || result.side == Side::South ?
                                       result.hitPos.y - std::floor(result.hitPos.y) : result.hitPos.x - std::floor(result.hitPos.x)
                                     };

                double texX = wallX * static_cast<double>(
                                  result.tileHit->tex[result.side]->getSize().x);

                if (((result.side == Side::North || result.side == Side::South) && dir.x > 0) ||
                        ((result.side == Side::East || result.side == Side::West) && dir.y < 0))
                {
                    texX = static_cast<double>(result.tileHit->tex[result.side]->getSize().x) - texX;
                }

                const unsigned char brightness = Utility::clamp(
                                                     t_map.ambientLight +
                                                     result.tileHit->brigthnessMap[result.side][static_cast<unsigned>(texX)]
                                                 , 0, 255);

                for (size_t j { static_cast<size_t>(drawStartClamped) }; static_cast<long>(j) < drawEndClamped; ++j)
                {
                    const double texY = Utility::map<double>(static_cast<double>(j),
                                                             static_cast<double>(drawStart),
                                                             static_cast<double>(drawEnd), 0,
                                                             static_cast<double>(result.tileHit->tex[result.side]->getSize().y));

                    sf::Color color;
                    if (bilinear_filtering)
                    {
                        color = Utility::bilinearFilter(*result.tileHit->tex[result.side], texX, texY);
                    }
                    else
                    {
                        color = result.tileHit->tex[result.side]->getPixel(texX, texY);
                        if (result.tileHit->decals[result.side] &&
                                (result.tileHit->decals[result.side]->getPixel(texX, texY).a
                                != 0xFE || true))
                        {
                            color = thor::blendColors(result.tileHit->tex[result.side]->getPixel(texX, texY),
                                    result.tileHit->decals[result.side]->getPixel(texX, texY),
                                    result.tileHit->decals[result.side]->getPixel(texX, texY).a / 255.f);
                        }
                    }
                    Utility::setBrightness(color, brightness);
                    color = detail::pixelIntensity(color, result.distance);
                    const size_t offset = (t_cam.screenSize().x * j + i) * 4;
                    if (BOOST_UNLIKELY(color.a != 0xFF))
                    {
                        sf::Color oldColor;
                        oldColor.r = renderTarget[offset + 0];
                        oldColor.g = renderTarget[offset + 1];
                        oldColor.b = renderTarget[offset + 2];

                        oldColor = detail::pixelIntensity(oldColor, result.distance);

                        if (result.side == Side::East || result.side == Side::West)
                        {
                            Utility::setBrightness(color, 150);
                        }

                        sf::Color blendedColor = thor::blendColors(oldColor, color, color.a / 255.f);

                        renderTarget[offset + 0] = blendedColor.r;
                        renderTarget[offset + 1] = blendedColor.g;
                        renderTarget[offset + 2] = blendedColor.b;
                        renderTarget[offset + 3] = 0xFF;
                    }
                    else
                    {
                        if (result.side == Side::East || result.side == Side::West)
                        {
                            Utility::setBrightness(color, 150);
                        }
                        renderTarget[offset + 0] = color.r;
                        renderTarget[offset + 1] = color.g;
                        renderTarget[offset + 2] = color.b;
                        renderTarget[offset + 3] = 0xFF;
                    }
                }

                sf::Vector2d floorWall {};
                switch (result.side)
                {
                    case Side::South:
                        floorWall.x = result.mapPos.x;
                        floorWall.y = result.mapPos.y + (1 - wallX);
                        break;

                    case Side::North:
                        floorWall.x = result.mapPos.x + 1;
                        floorWall.y = result.mapPos.y + (1 - wallX);
                        break;

                    case Side::East:
                        floorWall.x = result.mapPos.x + (1 - wallX);
                        floorWall.y = result.mapPos.y;
                        break;

                    case Side::West:
                        floorWall.x = result.mapPos.x + (1 - wallX);
                        floorWall.y = result.mapPos.y + 1;
                        break;
                }

                if (drawEndClamped < 0)
                {
                    drawEndClamped = static_cast<long>(t_cam.screenSize().y);
                }

                //detail::floorCasting(t_cam, t_map, result, wallX, bilinear_filtering, drawEndClamped, i, renderTarget);
            }
        }
    }


    // Sprite casting

    detail::spriteCasting(t_cam, t_map.sprites, zbuffer, t_bilinear_sprites, t_map.ambientLight, renderTarget);

    return renderTarget;
}

namespace detail
{

void spriteCasting(const Camera &t_cam, const std::vector<std::unique_ptr<DrawableActor> > &t_sprites, const std::vector<double> &t_zbuffer, bool t_bilinear_sprites, unsigned char t_ambientLight, std::vector<sf::Uint8> &t_image)
{
    const double invDet = 1.0 / (t_cam.plane().x * t_cam.direction().y -
                                 t_cam.direction().x * t_cam.plane().y); //required for correct matrix multiplication

    for (const auto& sprite_ref : t_sprites)
    {
        auto& sprite = *sprite_ref;
        sf::Image spriteTex = sprite.renderImage();

        sf::Vector2d spritePos = sprite.pos - t_cam.pos();

        //transform sprite with the inverse camera matrix
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]

        sf::Vector2d transform;
        transform.x = invDet * (t_cam.direction().y * spritePos.x - t_cam.direction().x * spritePos.y);
        transform.y = invDet * (-t_cam.plane().y * spritePos.x + t_cam.plane().x * spritePos.y);

        const int zScreen = (spriteTex.getSize().x * sprite.scale.y - sprite.z) / transform.y;

        int spriteScreenX = static_cast<int>((t_cam.screenSize().x / 2) * (1 + transform.x / transform.y));

        double ratioX = std::min<double>(spriteTex.getSize().x, spriteTex.getSize().y)
                        / std::max<double>(spriteTex.getSize().x, spriteTex.getSize().y);

        const int spriteHeight = static_cast<int>(std::abs(t_cam.screenSize().y / transform.y))
                                 * sprite.scale.y;

        int drawStartY = -spriteHeight / 2 + t_cam.screenSize().y / 2 + zScreen;
        if (drawStartY < 0)
        {
            drawStartY = 0;
        }
        size_t drawEndY = spriteHeight / 2 + t_cam.screenSize().y / 2 + zScreen;
        if (drawEndY >= t_cam.screenSize().y)
        {
            drawEndY = t_cam.screenSize().y - 1;
        }

        const int spriteWidth = std::abs(t_cam.screenSize().y / (transform.y)) * ratioX * sprite.scale.x;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
        {
            drawStartX = 0;
        }
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= int(t_cam.screenSize().x))
        {
            drawEndX = t_cam.screenSize().x - 1;
        }
        for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
        {
            double texX = (stripe - (-spriteWidth / 2 + spriteScreenX)) * double(spriteTex.getSize().x) / double(spriteWidth);

            if (transform.y > 0 && stripe < t_cam.screenSize().x && transform.y < t_zbuffer[stripe] && texX >= 0)
            {
                for (int y = drawStartY; y < drawEndY; ++y)
                {
                    const double d = (y - zScreen) - t_cam.screenSize().y / 2 + spriteHeight / 2;
                    const double texY = d * spriteTex.getSize().y / double(spriteHeight);
                    sf::Color color;
                    if (t_bilinear_sprites)
                    {
                        color = Utility::bilinearFilter(spriteTex, texX, texY);
                    }
                    else
                    {
                        color = spriteTex.getPixel(size_t(texX), size_t(texY));
                    }
                    Utility::setBrightness(color, t_ambientLight);
                    color = pixelIntensity(color, Utility::distance(t_cam.pos(), sprite.pos));
                    if (color.a == 255)
                    {
                        t_image[(t_cam.screenSize().x * y + stripe) * 4 + 0] = color.r;
                        t_image[(t_cam.screenSize().x * y + stripe) * 4 + 1] = color.g;
                        t_image[(t_cam.screenSize().x * y + stripe) * 4 + 2] = color.b;
                        t_image[(t_cam.screenSize().x * y + stripe) * 4 + 3] = 0xFF;
                    }
                }
            }
        }

    }
}

void floorCasting(const Camera &t_cam, const Map &t_map, const RaycastResult &t_result, double t_wallX, bool t_bilinear_filtering, long t_drawEnd, size_t t_screenX, std::vector<sf::Uint8> &t_image)
{

    sf::Vector2d floorWall {};
    switch (t_result.side)
    {
        case Side::South:
            floorWall.x = t_result.mapPos.x;
            floorWall.y = t_result.mapPos.y + (1 - t_wallX);
            break;

        case Side::North:
            floorWall.x = t_result.mapPos.x + 1;
            floorWall.y = t_result.mapPos.y + (1 - t_wallX);
            break;

        case Side::East:
            floorWall.x = t_result.mapPos.x + (1 - t_wallX);
            floorWall.y = t_result.mapPos.y;
            break;

        case Side::West:
            floorWall.x = t_result.mapPos.x + (1 - t_wallX);
            floorWall.y = t_result.mapPos.y + 1;
            break;
    }


    static Utility::LookupTable<double> lut(t_cam.screenSize().y, [&t_cam](size_t i)
    {
        return t_cam.screenSize().y / (2.f * i - t_cam.screenSize().y);
    });
#pragma omp parallel for simd
    for (size_t j { static_cast<size_t>(t_drawEnd) + 1 }; j < t_cam.screenSize().y; ++j)
    {
        //const double currentDist = t_cam.screenSize().y / (2.f * j - t_cam.screenSize().y);
        const double currentDist = lut[j];
        const double weight = currentDist / t_result.perpDistance;
        const sf::Vector2d floorPos = weight * floorWall +
                                      (1.0 - weight) * t_cam.pos();

        const auto sector = t_map.sectorAt(sf::Vector2s(floorPos));

        if (BOOST_LIKELY((bool)sector))
        {
            const sf::Image& floor = sector.get().floor;
            const sf::Image& ceiling = sector.get().floor;
            sf::Vector2d floorTexCoord {};

            floorTexCoord.x = (floorPos.x - std::floor(floorPos.x)) * floor.getSize().x;
            floorTexCoord.y = (floorPos.y - std::floor(floorPos.y)) * floor.getSize().y;

            sf::Vector2d ceilTexCoord {};

            ceilTexCoord.x = (floorPos.x - std::floor(floorPos.x)) * ceiling.getSize().x;
            ceilTexCoord.y = (floorPos.y - std::floor(floorPos.y)) * ceiling.getSize().y;

            sf::Color floorTexel;
            sf::Color ceilTexel;

            if (t_bilinear_filtering)
            {
                floorTexel = Utility::bilinearFilter(floor, floorTexCoord.x, floorTexCoord.y);
                ceilTexel = Utility::bilinearFilter(ceiling, ceilTexCoord.x, ceilTexCoord.y);
            }
            else
            {
                floorTexel = floor.getPixel(static_cast<unsigned>(floorTexCoord.x),
                                            static_cast<unsigned>(floorTexCoord.y));
                ceilTexel = ceiling.getPixel(static_cast<unsigned>(ceilTexCoord.x),
                                             static_cast<unsigned>(ceilTexCoord.y));
            }

            Utility::setBrightness(floorTexel, t_map.ambientLight);
            Utility::setBrightness(ceilTexel, t_map.ambientLight);

            Utility::setBrightness(floorTexel, 150);

            floorTexel = pixelIntensity(floorTexel, currentDist);
            ceilTexel = pixelIntensity(ceilTexel, currentDist);

            const auto floorPos = (t_cam.screenSize().x * (j-1) + t_screenX) * 4;
            t_image[floorPos + 0] = floorTexel.r;
            t_image[floorPos + 1] = floorTexel.g;
            t_image[floorPos + 2] = floorTexel.b;
            t_image[floorPos + 3] = floorTexel.a;

            const auto ceilPos = (t_cam.screenSize().x * (t_cam.screenSize().y - j) + t_screenX) * 4;
            t_image[ceilPos + 0] = ceilTexel.r;
            t_image[ceilPos + 1] = ceilTexel.g;
            t_image[ceilPos + 2] = ceilTexel.b;
            t_image[ceilPos + 3] = ceilTexel.a;
        }
    }
}

sf::Color pixelIntensity(sf::Color t_pixel, double t_dist)
{
    sf::Color returnColor = t_pixel;
    if (t_dist < 0) t_dist = 0;
    unsigned multiplier = 10;
    double intensity = 0.5 / t_dist * multiplier;
    if (intensity > 1) intensity = 1;
    else if (intensity < 0) intensity = 0;

    returnColor.r = t_pixel.r * intensity;
    returnColor.g = t_pixel.g * intensity;
    returnColor.b = t_pixel.b * intensity;

    return returnColor;
}

}

bool rayIntersectsSprite(const sf::Vector2d &t_begin, const sf::Vector2d &t_dir, Map &t_map,
                         DrawableActor*& t_hitSprite, size_t t_maxDistance)
{
    typedef boost::geometry::model::d2::point_xy<double> Point;
    boost::geometry::model::linestring<Point> line;
    Point origin = {t_begin.x, t_begin.y};
    line.push_back(origin);
    line.push_back({t_begin.x + t_dir.x * t_maxDistance, t_begin.y + t_dir.y * t_maxDistance});

    for (auto& sprite : t_map.sprites)
    {
        sf::Image img = sprite->renderImage();
        boost::geometry::model::polygon<Point> spriteRect;
        double largestSide = std::max<double>(img.getSize().x, img.getSize().y);
        double side = img.getSize().x / largestSide;
        std::vector<Point> points =
        {
            {sprite->pos.x - side / 2, sprite->pos.x - side / 2},
            {sprite->pos.x - side / 2, sprite->pos.x + side / 2},
            {sprite->pos.x + side / 2, sprite->pos.x + side / 2},
            {sprite->pos.x + side / 2, sprite->pos.x - side / 2}
        };
        boost::geometry::assign_points(spriteRect, points);
        boost::geometry::model::multi_point<Point> intersectionPoints;
        if (boost::geometry::intersection(line, spriteRect, intersectionPoints) && !intersectionPoints.empty())
        {
            auto result = castRay(t_begin, t_dir, t_map, HitMode::Visibility).front();
            if (boost::geometry::distance(origin, intersectionPoints.front()) <= result.distance)
            {
                t_hitSprite = sprite.get();
                return true;
            }
        }
    }

    return false;
}

} // namespace Raycasting

} // namespace Rayfun
