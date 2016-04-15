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

namespace Rayfun
{

inline std::unique_ptr<State> makeState(StateID t_stateID)
{
//    switch (t_stateID)
//    {
//        case StateID::Menu:
//            return std::make_unique<State>();
//        case StateID::Game:
//            return std::make_unique<State>();
//    }
}

} // namespace Rayfun

#endif // STATEFACTORY_HPP
