/*  gameplayutility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 04/06/2016
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

#include "utils/gameplayglobals.hpp"

#include <atomic>
#include <thread>

#include <boost/optional.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Sleep.hpp>

#include <Thor/Math/Trigonometry.hpp>
#include <Thor/Math/Random.hpp>

#include "actor.hpp"

#include "raycasting.hpp"
#include "utils/mathutility.hpp"
#include "utils/graphicsutility.hpp"
#include "map.hpp"

#include "constants.hpp"

#include <iostream>

namespace Rayfun
{

static std::atomic_size_t g_soundNumber { 0 };

boost::optional<Actor&> fireBullets(Actor &t_actor, const sf::Vector2d &t_src, const sf::Vector2d &t_dir,
                                    const sf::Vector2d &t_spread, Map& t_map, const State::Context &t_context,
                                    size_t t_damage, size_t t_num, const std::string &t_dmgType,
                                    size_t t_maxDistance)
{
    std::sort(t_map.sprites.begin(), t_map.sprites.end(), [&t_actor](std::unique_ptr<DrawableActor>& rhs,
              std::unique_ptr<DrawableActor>& lhs){
        return Utility::distance(rhs->pos, t_actor.pos) < Utility::distance(lhs->pos, t_actor.pos);
    });

    for (size_t i { 0 }; i < t_num; ++i)
    {
        sf::Vector2d unitVector = Utility::angleToVector<double>(thor::random(0, 360));
        double spreadIntensity = thor::random(0.f, 1.f);
        sf::Vector2d dev { t_spread.x * spreadIntensity, t_spread.y * spreadIntensity };
        dev = thor::cwiseProduct(unitVector, dev);

        DrawableActor* actor = nullptr;
        if (Raycasting::rayIntersectsSprite(t_src, Utility::angleToVector(dev.x + thor::polarAngle(t_dir)),
                                            t_map, actor))
        {
            if (actor && actor->ondamage)
            {
                actor->ondamage(t_actor, *actor, t_damage, t_dmgType);
            }
            //TODO: implement puffs ?
        }
        else
        {
            auto result = Raycasting::castRay(t_src, Utility::angleToVector(dev.x + thor::polarAngle(t_dir)),
                                              t_map, Raycasting::HitMode::Visibility).back();

            if (result.tileHit && result.tileHit->tex[result.side] != 0)
            {
                if (result.distance > t_maxDistance)
                {
                    return boost::none;
                }

                if (!result.tileHit->decals[result.side])
                {
                    result.tileHit->decals[result.side] = sf::Image();
                }
                auto& decal = *(result.tileHit->decals[result.side]);

                // FIXME : again find a way to get texture size by ID !

                if (true /*decal.getSize() != {64, 64 }*/)
                {
                    decal.create(64,
                            64, sf::Color::Transparent);
                }

                double wallX { 0 };
                if (result.side == Side::North || result.side == Side::South)
                {
                    wallX = result.hitPos.y - std::floor(result.hitPos.y);
                }
                else
                {
                    wallX = result.hitPos.x - std::floor(result.hitPos.x);
                }
                if (result.side == Side::South || result.side == Side::West)
                {
                    wallX = 1 - wallX;
                }
                wallX *= decal.getSize().x;
                double wallY = decal.getSize().y / 2.f + (result.distance / 2.f) * std::tan(thor::toRadian(dev.y))
                               * decal.getSize().y;
                if (result.tileHit && wallY >= 0 && wallY <= decal.getSize().y)
                {
                    Utility::addDecal(decal, t_context.resources.imageHolder["decals/bullet"],
                            sf::Vector2s(wallX, wallY), true);
                    if (result.tileHit->ondamage[result.side])
                    {
                        result.tileHit->ondamage[result.side](t_actor, *result.tileHit, t_damage, t_dmgType);
                    }
                }
            }
        }
    }

    return boost::none;
}

void playSoundAt(const Resources &t_res, const std::string &t_sound,
                 const sf::Vector2f& t_pos, float t_volume)
{
    if (g_soundNumber < c_maxSounds)
    {
        std::thread([&t_res, t_volume, t_sound, t_pos]{
            ++g_soundNumber;
            sf::Sound *sound = new sf::Sound;
            sound->setBuffer(t_res.soundHolder[t_sound]);
            sound->setVolume(t_volume);
            sound->setPosition(t_pos.x, t_pos.y, 0);
            sound->play();
            while (sound->getStatus() == sf::Sound::Playing)
            {sf::sleep(sf::seconds(0.1f));};
            delete sound;
            --g_soundNumber;
        }).detach();
    }
}

void playSoundUI(const Resources &t_res, const std::string &t_sound, float t_volume)
{
    if (g_soundNumber < c_maxSounds)
    {
        std::thread([&t_res, t_volume, t_sound]{
            ++g_soundNumber;
            sf::Sound *sound = new sf::Sound;
            sound->setBuffer(t_res.soundHolder[t_sound]);
            sound->setVolume(t_volume);
            sound->setRelativeToListener(true);
            sound->setPosition(0, 0, 0);
            sound->play();
            while (sound->getStatus() == sf::Sound::Playing)
            {sf::sleep(sf::seconds(0.1f));};
            delete sound;
            --g_soundNumber;
        }).detach();
    }
}

bool spriteClipAt(const sf::Vector2d &t_pos, Map &t_map, DrawableActor *&t_hitSprite)
{
    typedef boost::geometry::model::d2::point_xy<double> Point;
    Point origin = {t_pos.x, t_pos.y};

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
        if (boost::geometry::intersects(origin, spriteRect))
        {
            if (sprite->clip)
            {
                t_hitSprite = sprite.get();
                return true;
            }
        }
    }

    return false;
}

}
