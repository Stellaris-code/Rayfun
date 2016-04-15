/*  common %{Cpp:License:ClassName} - Yann BOUCHER (yann) 11/04/2016
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
#ifndef COMMON_HPP
#define COMMON_HPP

#include <map>

#include <SFML/System/Vector2.hpp>

namespace sf
{

using Vector2d = sf::Vector2<double>;
using Vector2s = sf::Vector2<size_t>;

}

namespace Rayfun
{

enum class Side
{
    North,
    South,
    West,
    East
};

template <typename T>
class Directional
{
    public:
        Directional() = default;

        explicit Directional(T t_data)
        { set(t_data); }

        void set(const T& t_data)
        {
            m_sides[Side::North] = t_data;
            m_sides[Side::South] = t_data;
            m_sides[Side::East] = t_data;
            m_sides[Side::West] = t_data;
        }

        T& operator[](Side side)       { return m_sides[side]; }
        const T& operator[](Side side) const { return m_sides.at(side); }

    private:
        std::map<Side, T> m_sides;
};

} // namespace Rayfun

#endif // COMMON_HPP
