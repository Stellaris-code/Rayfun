/*  src/statemachine.cpp StateMachine - Yann BOUCHER (yann) 11/08/2016
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
#include "statemachine.hpp"

#include "statefactory.hpp"

namespace Rayfun
{

StateMachine::StateMachine(State::Context &t_contextObject)
    : m_contextObject(t_contextObject)
{

}

void StateMachine::changeState(StateID t_state)
{
    if (!m_states.empty())
    {
        popState();
    }

    pushState(t_state);
}

void StateMachine::pushState(StateID t_state)
{
    m_states.push(std::pair<chaiscript::ChaiScript::State, std::unique_ptr<State>>(
                      m_contextObject.scriptEngine.get_state(),
                      makeState(t_state, m_contextObject)));
    m_changed = true;
}

void StateMachine::popState()
{
    if (m_states.empty())
    {
        throw_statemachine_error("Tried to pop empty state stack !");
    }

    m_contextObject.scriptEngine.set_state(m_states.top().first);
    m_states.pop();
    m_changed = true;
}

}
