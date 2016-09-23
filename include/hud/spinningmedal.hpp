/*  include/hud/spinningmedal.hpp SpinningMedal - Yann BOUCHER (yann) 01/09/2016
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
#ifndef SPINNINGMEDAL_HPP
#define SPINNINGMEDAL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/System/Time.hpp>

#include <SelbaWard/SpinningCard.hpp>

namespace Rayfun
{

class SpinningMedal : public sf::Drawable, public sf::Transformable
{
    public:
        SpinningMedal(const sf::Texture& t_medalTexture);

    public:
        void update(const sf::Time& t_dt);

    public:
        double speed { 360 };

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        sf::Sprite m_medalSprite;
        sw::SpinningCard m_medal { m_medalSprite };
        double m_spinAmount { 0 };
};

}

#endif // SPINNINGMEDAL_HPP
