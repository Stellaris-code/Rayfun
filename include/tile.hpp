/*  tile %{Cpp:License:ClassName} - Yann BOUCHER (yann) 11/04/2016
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
#ifndef TILE_HPP
#define TILE_HPP

#include <functional>
#include <memory>
#include <vector>

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Time.hpp>

#include "common.hpp"

#include "taggable.hpp"
#include "actor.hpp"

namespace Rayfun
{

struct Tile : public Taggable
{
        bool isWall { false };

        Directional<bool> clip { false };

        Directional<unsigned char> tex {}; // texture IDs

        Directional<boost::optional<sf::Image>> decals {};

        Directional<std::function<void(Actor&, Tile&, size_t, std::string)>> ondamage {};
        Directional<std::function<void(Actor&, Tile&)>> ontrigger {};

        sf::Time slideSpeed { sf::Time::Zero };
        sf::Time slideProgress { sf::Time::Zero };
        bool sliding { false };
        Side slideSide { Side::North };
        Side slideDirection { Side::East };

        Directional<std::vector<unsigned char>> brigthnessMap;
};

} // namespace Rayfun

#endif // TILE_HPP
