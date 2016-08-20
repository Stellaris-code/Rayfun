/*  statefactory %{Cpp:License:ClassName} - Yann BOUCHER (yann) 15/04/2016
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
#ifndef STATEFACTORY_HPP
#define STATEFACTORY_HPP

#include <memory>

#include "stateid.hpp"
#include "state.hpp"
#include "states/gamestate.hpp"
#include "states/menustate.hpp"
#include "states/introstate.hpp"

namespace Rayfun
{

inline std::unique_ptr<State> makeState(StateID t_stateID, State::Context& t_context)
{
    switch (t_stateID)
    {
        case StateID::Menu:
            return std::unique_ptr<State>(std::make_unique<MenuState>(t_context));
        case StateID::Intro:
            return std::unique_ptr<State>(std::make_unique<IntroState>(t_context));
        case StateID::Game:
            return std::unique_ptr<State>(std::make_unique<GameState>(t_context));
    }
}

} // namespace Rayfun

#endif // STATEFACTORY_HPP
