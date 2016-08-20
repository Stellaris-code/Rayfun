/*  include/melttransition.hpp MeltTransition - Yann BOUCHER (yann) 13/08/2016
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
#ifndef MELTTRANSITION_HPP
#define MELTTRANSITION_HPP

#include <stdexcept>
#include <vector>

#include <SFML/System/Time.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class screenmelt_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_screenmelt_error(const std::string& t_why)
{
    throw screenmelt_error("Screen melt transition error : " + t_why);
}

class MeltTransition : public sf::Drawable, public sf::Transformable
{
    public:
        MeltTransition(const sf::Texture& t_tex,
                       sf::Time t_animDuration = sf::seconds(1.5), unsigned t_stripeSize = 4)
            : m_tex(&t_tex)
        {
            reset(t_tex, t_animDuration, t_stripeSize);
        }

        MeltTransition() = default;

        void reset(const sf::Texture& t_tex,
                   sf::Time t_animDuration = sf::seconds(1.5), unsigned t_stripeSize = 4);

        void update(sf::Time t_deltaTime);

        bool completed() const;

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        void createStripes();
        void checkTextureIsValid() const;

    private:
        const sf::Texture* m_tex { nullptr };
        sf::Time m_animDuration;
        unsigned m_stripeSize;
        std::vector<sf::Sprite> m_stripes;
        std::vector<float> m_delayMap;
        std::vector<bool> m_completedMap;
};

#endif // MELTTRANSITION_HPP
