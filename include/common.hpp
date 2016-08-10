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

#include <SFML/System/Vector2.hpp>
#include <Thor/Resources/ResourceHolder.hpp>

namespace sf
{

using Vector2d = sf::Vector2<double>;
using Vector2s = sf::Vector2<size_t>;

}


namespace selbaward
{
class BitmapFont;
}

namespace sf
{
class RenderWindow;
class Event;
class Time;
class Texture;
class Image;
class Font;
class SoundBuffer;
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

struct Resources
{
        thor::ResourceHolder<sf::Texture, std::string>& textureHolder;
        thor::ResourceHolder<sf::Image, std::string>& imageHolder;
        thor::ResourceHolder<sf::Font, std::string>& fontHolder;
        thor::ResourceHolder<selbaward::BitmapFont, std::string>& bitmapFontHolder;
        thor::ResourceHolder<sf::SoundBuffer, std::string>& soundHolder;
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
            m_north = t_data;
            m_south = t_data;
            m_east = t_data;
            m_west = t_data;
        }

        T& operator[](Side side)
        {
            switch (side)
            {
                case Side::North:
                    return m_north;
                case Side::South:
                    return m_south;
                case Side::East:
                    return m_east;
                case Side::West:
                    return m_west;
            }
        }
        const T& operator[](Side side) const
        {
            switch (side)
            {
                case Side::North:
                    return m_north;
                case Side::South:
                    return m_south;
                case Side::East:
                    return m_east;
                case Side::West:
                    return m_west;
            }
        }

    private:
        T m_north;
        T m_south;
        T m_east;
        T m_west;
};



} // namespace Rayfun

#endif // COMMON_HPP
