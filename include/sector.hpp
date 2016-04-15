/*  sector %{Cpp:License:ClassName} - Yann BOUCHER (yann) 13/04/2016
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
#ifndef SECTOR_HPP
#define SECTOR_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Image.hpp>

namespace Rayfun
{

struct Sector
{
        sf::Rect<size_t> rect;
        sf::Image floor;
        sf::Image ceiling;
};

} // namespace Rayfun

#endif // SECTOR_HPP
