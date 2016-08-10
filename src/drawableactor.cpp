/*  src/drawableactor.cpp DrawableActor - Yann BOUCHER (yann) 13/06/2016
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
#include "drawableactor.hpp"

#include <cmath>

#include <Thor/Math/Trigonometry.hpp>

#include "mathutility.hpp"

namespace Rayfun
{

double DrawableActor::playerFacingAngle() const
{
    return Utility::normalizeAngle(thor::toDegree(std::atan2(playerPos.y - pos.y, playerPos.x - pos.x)) + angle + 180);
}

}
