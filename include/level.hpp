/*  level %{Cpp:License:ClassName} - Yann BOUCHER (yann) 15/04/2016
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
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>

#include <SFML/System/Time.hpp>

#include <SFML/Audio/Music.hpp>

#include <SFML/Graphics/Texture.hpp>

#include "map.hpp"

namespace Rayfun
{

struct Level : sf::NonCopyable
{
        Level() = default;
        Level(const sf::Vector2s& t_mapSize)
            : map(t_mapSize)
        {}

        Level(Level&&) {};
        Level& operator = (Level&&) = default;

        Map map { sf::Vector2s { 64, 64 } };
        std::string name {};
        sf::Time parTime {};
        sf::Music music {};
        bool intro { false };
        std::string beginMessage {};
        sf::Texture beginBkg {};
        bool outro { false };
        std::string endMessage {};
        sf::Texture endBkg {};
};

}

#endif // LEVEL_HPP
