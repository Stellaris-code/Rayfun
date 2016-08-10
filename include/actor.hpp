/*  include/actor.hpp Actor - Yann BOUCHER (yann) 11/05/2016
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
#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include "state.hpp"

#include "common.hpp"

namespace Rayfun
{

class Actor : private sf::NonCopyable
{
    public:
        explicit Actor(State::Context& t_context)
            : m_context(t_context)
        {}
        virtual ~Actor() {}

    public:
        virtual void update(const sf::Time&) {}

    public:
        sf::Vector2d pos { 0, 0 };
        double angle { 0 };

    protected:
        State::Context& m_context;

    private:
};

}

#endif // ACTOR_HPP
