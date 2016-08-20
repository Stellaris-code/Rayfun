/*  glslinterop %{Cpp:License:ClassName} - Yann BOUCHER (yann) 18/08/2016
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

#include "utils/glslinterop.hpp"

#include <cmath>

#include <SFML/Graphics/Texture.hpp>

#include "map.hpp"

namespace Rayfun
{

namespace detail
{

void setTexIndex(std::size_t i, std::size_t j, const std::array<unsigned char, 4> &texIndex, sf::Image &img)
{
    sf::Color pixel;
    pixel.r = texIndex[0];
    pixel.g = texIndex[1];
    pixel.b = texIndex[2];
    pixel.a = texIndex[3];

    img.setPixel(i, j, pixel);
}

}

sf::Texture mapToTexture(const Map &t_map)
{
    sf::Image img;
    img.create(t_map.size().x, t_map.size().y);

    for (size_t i { 0 }; i < t_map.size().x; ++i)
    {
        for (size_t j { 0 }; j < t_map.size().y; ++j)
        {
            detail::setTexIndex(i, j, {{0, 0, 0, 0}}, img);
        }
    }

    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

}
