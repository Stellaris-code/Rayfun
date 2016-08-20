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
#ifndef GAMEPLAYUTILITY_HPP
#define GAMEPLAYUTILITY_HPP

#include <string>
#include <vector>
#include <functional>
#include <limits>

#include <SFML/System/Vector2.hpp>

#include "state.hpp"
#include "common.hpp"
#include "drawableactor.hpp"

namespace boost
{
template <typename T>
class optional;
}

namespace Rayfun
{


class Map;
class Actor;

bool spriteClipAt(const sf::Vector2d &t_pos, Map& t_map, DrawableActor *&t_hitSprite);

boost::optional<Actor &> fireBullets(Actor& t_actor, const sf::Vector2d &t_src,
                                   const sf::Vector2d &t_dir, const sf::Vector2d &t_spread, Map &t_map,
                                   const State::Context& t_context,
                                   size_t t_damage, size_t t_num = 1,
                                   const std::string& t_dmgType = "bullet",
                 size_t t_maxDistance = std::numeric_limits<size_t>::max());

void playSoundAt(const Resources &t_res, const std::string& t_sound, const sf::Vector2f &t_pos,
                 float t_volume = 100);

void playSoundUI(const Resources &t_res, const std::string& t_sound, float t_volume = 100);

}

#endif // GAMEPLAYUTILITY_HPP
