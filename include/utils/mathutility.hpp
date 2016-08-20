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
#ifndef MATHUTILITY_HPP
#define MATHUTILITY_HPP

#include <cmath>

namespace sf
{

template <typename T>
class Vector2;

}

namespace Rayfun
{
namespace Utility
{

template <typename T>
inline sf::Vector2<T> angleToVector(T t_angle);

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

template <typename T>
inline T clamp(T x, T lower, T upper)
{
    if (x < lower)
    {
        x = lower;
    }
    else if (x > upper)
    {
        x = upper;
    }

    return x;
}

template <typename T>
inline T wrap(T kX, T kLowerBound, T kUpperBound)
{
    T range_size = kUpperBound - kLowerBound + T(1);

    if (kX < kLowerBound)
        kX += range_size * ((kLowerBound - kX) / range_size + T(1));

    return std::fmod(kLowerBound + (kX - kLowerBound), range_size);
}

template <typename T>
inline T normalizeAngle(T angle)
{
    return wrap(angle, T(0), T(360));
}

template <typename T>
inline bool nearEqual(T a, T b, T tolerance = T(1e-5))
{
    return (a - b) * (a - b) < tolerance * tolerance;
}

}
}

#include "mathutility.tpp"

#endif // MATHUTILITY_HPP
