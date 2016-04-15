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

#include "camera.hpp"

#include <cmath>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

namespace Rayfun
{

Camera::Camera(const sf::Vector2s &t_screenSize, const sf::Vector2d& t_pos, const sf::Vector2d &t_plane,
               const sf::Vector2d &t_direction)
{
    setPos(t_pos);
    setScreenSize(t_screenSize);
    setPlane(t_plane);
    setDirection(t_direction);
}

Camera::Camera(const sf::Vector2s& t_screenSize, const sf::Vector2d& t_pos, double t_fov, double t_angle)
{
    setPos(t_pos);
    setScreenSize(t_screenSize);
    setFOV(t_fov);
    setAngle(t_angle);
}

void Camera::setFOV(double t_fov)
{
    m_plane = sf::Vector2d(0, std::tan(thor::toRadian(t_fov / 2.f)));
}

void Camera::setAngle(double t_angle)
{
    thor::rotate(m_plane, t_angle - angle());
    m_direction = sf::Vector2d(std::cos(thor::toRadian(t_angle)), std::sin(thor::toRadian(t_angle)));
}

void Camera::setPlane(const sf::Vector2d& t_plane)
{
    m_plane = t_plane;
}

void Camera::setDirection(const sf::Vector2d &t_direction)
{
    thor::rotate(m_plane, thor::polarAngle(t_direction) - angle());
    m_direction = t_direction;
}

void Camera::setPos(const sf::Vector2d& t_pos)
{
    m_pos = t_pos;
}

void Camera::setScreenSize(const sf::Vector2s& t_size)
{
    m_screenSize = t_size;
}

double Camera::fov() const
{
    return 2 * thor::toDegree(std::atan(m_plane.y));
}

double Camera::angle() const
{
    return thor::toDegree(std::atan2(m_direction.y, m_direction.x));
}

sf::Vector2d Camera::plane() const
{
    return m_plane;
}

sf::Vector2d Camera::direction() const
{
    return m_direction;
}

sf::Vector2d Camera::pos() const
{
    return m_pos;
}

sf::Vector2s Camera::screenSize() const
{
    return m_screenSize;
}

} // namespace Rayfun
