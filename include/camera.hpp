/*  camera Camera - Yann BOUCHER (yann) 11/04/2016
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
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/System/Vector2.hpp>

#include "common.hpp"

namespace Rayfun
{

class Camera
{
    public:
        Camera(const sf::Vector2s& t_screenSize, const sf::Vector2d& t_pos, const sf::Vector2d& t_plane,
               const sf::Vector2d& t_direction);
        Camera(const sf::Vector2s& t_screenSize, const sf::Vector2d& t_pos, double t_fov, double t_angle);

    public:
        void setFOV(double t_fov);
        void setAngle(double t_angle);

        void setPlane(const sf::Vector2d& t_plane);
        void setDirection(const sf::Vector2d& t_direction);

        void setPos(const sf::Vector2d& t_pos);
        void setScreenSize(const sf::Vector2s& t_size);


        double fov() const;
        double angle() const;

        const sf::Vector2d& plane() const;
        const sf::Vector2d& direction() const;

        const sf::Vector2d& pos() const;
        const sf::Vector2s& screenSize() const;

    private:
        sf::Vector2d m_pos {};
        sf::Vector2s m_screenSize {};
        sf::Vector2d m_plane {};
        sf::Vector2d m_direction {};
};

} // namespace Rayfun

#endif // CAMERA_HPP
