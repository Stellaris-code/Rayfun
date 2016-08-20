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
#ifndef GLSLINTEROP_HPP
#define GLSLINTEROP_HPP

#include <array>

namespace sf
{
class Texture;
class Image;
}

namespace Rayfun
{

class Map;

namespace detail
{
void setTexIndex(std::size_t i, std::size_t j, const std::array<unsigned char, 4>& texIndex, sf::Image& img);
}

sf::Texture mapToTexture(const Map& t_map);

}

#endif // GLSLINTEROP_HPP
