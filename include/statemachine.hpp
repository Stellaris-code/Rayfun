/*  include/statemachine.hpp StateMachine - Yann BOUCHER (yann) 11/08/2016
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
#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <memory>
#include <stack>

#include <chaiscript/chaiscript.hpp>

#include "stateid.hpp"
#include "state.hpp"

namespace Rayfun
{

class State;

class statemachine_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_statemachine_error(const std::string& t_why)
{
    throw statemachine_error("State machine error : " + t_why);
}

class StateMachine
{
    public:
        StateMachine(State::Context& t_contextObject);

        void changeState(StateID t_state);
        void pushState(StateID t_state);
        void popState();

        const std::unique_ptr<State>& top() const
        { return m_states.top().second; }
        std::unique_ptr<State>& top()
        { return m_states.top().second; }

        bool empty() const
        { return m_states.empty(); }
        bool changed() const
        {
            bool changed = m_changed;
            m_changed = false;
            return changed;
        }

    private:
        std::stack<std::pair<chaiscript::ChaiScript::State, std::unique_ptr<State>>> m_states;
        State::Context& m_contextObject;
        mutable bool m_changed { false };
};

}

#endif // STATEMACHINE_HPP
