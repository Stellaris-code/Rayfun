/*  include/states/introstate.hpp IntroState - Yann BOUCHER (yann) 11/08/2016
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
#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SFML/Graphics/Sprite.hpp>

#include "state.hpp"

namespace Rayfun
{

class IntroState : public State
{
    public:
        explicit IntroState(State::Context& t_context);

    public:
        virtual void pause() override;
        virtual void resume() override;


        virtual void handleEvent(const sf::Event&) override;

        virtual void update(const sf::Time&) override;

        virtual void display() override;

    private:
        void drawImgui();

    private:
        sf::Sprite m_bckgSprite;
};

}

#endif // INTROSTATE_HPP
