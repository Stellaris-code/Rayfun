/*  include/drawableactor.hpp DrawableActor - Yann BOUCHER (yann) 13/06/2016
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
#ifndef DRAWABLEACTOR_HPP
#define DRAWABLEACTOR_HPP

#include <functional>

#include "actor.hpp"

namespace sf
{
class Image;
}

namespace Rayfun
{

class DrawableActor : public Actor
{
        using Actor::Actor;

    public:
        virtual sf::Image renderImage() const = 0;

    protected:
        double playerFacingAngle() const;

    public:
        sf::Vector2d playerPos { 0, 0 };
        sf::Vector2d scale { 1, 1 };
        double z { 0 };
        bool clip { true };

        std::function<void(Actor&, DrawableActor&, size_t, std::string)> ondamage {};
        std::function<void(Actor&, DrawableActor&)> ontrigger {};
};

}

#endif // DRAWABLEACTOR_HPP
