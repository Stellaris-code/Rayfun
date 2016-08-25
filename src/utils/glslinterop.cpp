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

#include "map.hpp"

namespace Rayfun
{

namespace Utility
{


std::vector<uint8_t> mapToBuffer(const Map &t_map)
{
    std::vector<uint8_t> buffer;

    for (size_t i { 0 }; i < t_map.size().x; ++i)
    {
        for (size_t j { 0 }; j < t_map.size().y; ++j)
        {
            const auto& tileAt = t_map.tileAt({i, j});
            buffer.push_back(tileAt.tex[Side::North]);
            buffer.push_back(tileAt.tex[Side::South]);
            buffer.push_back(tileAt.tex[Side::East]);
            buffer.push_back(tileAt.tex[Side::West]);
        }
    }

    return buffer;
}

}

}
