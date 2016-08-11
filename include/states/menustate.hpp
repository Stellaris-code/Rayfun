/*  include/menustate.hpp MenuState - Yann BOUCHER (yann) 10/08/2016
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
#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <functional>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "state.hpp"

namespace Rayfun
{

class MenuState : public State
{
    public:
        explicit MenuState(State::Context& t_context);

    public:
        virtual void pause() override;
        virtual void resume() override;


        virtual void handleEvent(const sf::Event&) override;

        virtual void update(const sf::Time&) override;

        virtual void display() override;

        virtual void initChai() override;

    private:
        enum SelectedOption
        {
            Play = 0,
            Exit,
            NoAction
        };

    private:
        void loadMenuFile();

    private:
        std::function<SelectedOption()> m_drawCallback;
        sf::Sprite m_bckgSprite;
        sf::Texture m_background;
};

}

#endif // MENUSTATE_HPP
