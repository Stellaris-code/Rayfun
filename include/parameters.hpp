/*  parameters %{Cpp:License:ClassName} - Yann BOUCHER (yann) 08/05/2016
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
#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <SFML/System/Vector2.hpp>

#include <Thor/Input/ActionMap.hpp>

#include "common.hpp"

namespace Rayfun
{

struct Parameters
{
        double fov { 90 };
        thor::ActionMap<std::string> bindings;
        bool bilinear_filtering = false;
        bool bilinear_sprites = false;
};

} // namespace Rayfun

#endif // PARAMETERS_HPP
