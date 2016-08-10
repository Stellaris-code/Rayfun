/*  include/billboard.hpp Billboard - Yann BOUCHER (yann) 14/06/2016
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
#ifndef BILLBOARD_HPP
#define BILLBOARD_HPP

#include <vector>

#include <SFML/Graphics/Image.hpp>

#include "drawableactor.hpp"

namespace Rayfun
{

class Billboard : public DrawableActor
{
        using DrawableActor::DrawableActor;

    public:
        virtual ~Billboard() {}

    public:
        void setImage(const sf::Image& t_img, double t_minAngle = 0, double t_maxAngle = 360);

    public:
        virtual sf::Image renderImage() const override;

    private:
        std::vector<std::pair<std::pair<double, double>, sf::Image>> m_sprites;
};

}

#endif // BILLBOARD_HPP
