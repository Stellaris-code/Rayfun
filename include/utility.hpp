/*  utility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 12/04/2016
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
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cmath>

#include <SFML/System/Vector2.hpp>

namespace Rayfun
{

namespace Utility
{

template <typename T>
inline T distance(const sf::Vector2<T>& t_lhs, const sf::Vector2<T>& t_rhs)
{
    return std::hypot(t_lhs.x - t_rhs.x, t_lhs.y - t_rhs.y);
}

template <typename T>
inline T map(T x, T in_min, T in_max, T out_min, T out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

}

} // namespace Rayfun

#endif // UTILITY_HPP
