/*  mathutility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 16/05/2016
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
#ifndef MATHUTILITY_TPP
#define MATHUTILITY_TPP

#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <Thor/Vectors.hpp>

namespace Rayfun
{
namespace Utility
{

template <typename T>
inline sf::Vector2<T> angleToVector(T t_angle)
{
    return sf::Vector2<T>(std::cos(thor::toRadian(t_angle)), std::sin(thor::toRadian(t_angle)));
}

}
}

#endif // MATHUTILITY_TPP
