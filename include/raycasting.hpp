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

#include <functional>
#include <vector>

#include <boost/optional.hpp>

#include <SFML/Config.hpp>

#include "common.hpp"

#include "tile.hpp"

namespace Rayfun
{

class Camera;
class Map;
class DrawableActor;

namespace Raycasting
{

struct RaycastResult
{
        const Side side { Side::North };
        boost::optional<Tile&> tileHit {};
        const sf::Vector2d hitPos {};
        const sf::Vector2i mapPos {};
        const double distance { 0 };
        const double perpDistance { 0 };
};

enum class HitMode
{
    Visibility,
    Clipping
};

class raycasting_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_raycasting_error(const std::string& t_why)
{
    throw raycasting_error("Error during : " + t_why);
}

bool rayIntersectsSprite(const sf::Vector2d &t_begin, const sf::Vector2d &t_dir, Map &t_map,
                         DrawableActor *&t_hitSprite, size_t t_maxDistance = 9999999999);

std::vector<RaycastResult> castRay(const sf::Vector2d& t_begin, const sf::Vector2d& t_dir, Map &t_map,
                                   HitMode t_hitmode = HitMode::Visibility);

std::vector<sf::Uint8> render(const Camera& t_cam, const Map& t_map,
                              bool bilinear_filtering = false, bool t_bilinear_sprites = false);

namespace detail
{
void spriteCasting(const Camera& t_cam, const std::vector<std::unique_ptr<DrawableActor>>& t_sprites,
                   const std::vector<double>& t_zbuffer, bool t_bilinear_sprites, unsigned char t_ambientLight,
                   std::vector<sf::Uint8>& t_image);

void floorCasting(const Camera& t_cam, const Map& t_map, const RaycastResult& t_result, double t_wallX,
                  bool t_bilinear_filtering, long t_drawEnd, size_t t_screenX,
                  std::vector<sf::Uint8>& t_image);

sf::Color pixelIntensity(sf::Color t_pixel, double t_dist);
}

}

}

#endif // RAYCASTING_HPP
