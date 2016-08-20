/*  src/lightsource.cpp LightSource - Yann BOUCHER (yann) 05/06/2016
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
#include "lightsource.hpp"

#include "utils/mathutility.hpp"
#include "raycasting.hpp"

#include "pakcontents.hpp"

namespace Rayfun
{

void LightSource::update(const sf::Time &)
{
    for (double d { 0 }; d < 360; d+=0.01)
    {
        auto result = Raycasting::castRay(pos, Utility::angleToVector<double>(d),
                                         m_context.mapPack.level.map).back();
        if (result.tileHit)
        {
            double wallX { 0 };
            if (result.side == Side::North || result.side == Side::South)
            {
                wallX = result.hitPos.y - std::floor(result.hitPos.y);
            }
            else
            {
                wallX = result.hitPos.x - std::floor(result.hitPos.x);
            }
            if (result.side == Side::South || result.side == Side::West)
            {
                wallX = 1 - wallX;
            }
            wallX *= result.tileHit->tex[result.side]->getSize().x;
            result.tileHit->brigthnessMap[result.side][size_t(wallX)] = 255 / result.distance;
        }
    }
}

}
