/*  include/state.hpp State - Yann BOUCHER (yann) 15/04/2016
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
#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <memory>

#include <SFML/System/NonCopyable.hpp>

namespace chaiscript
{
class ChaiScript;
}

namespace sf
{
class RenderWindow;
class Event;
class Time;
}

namespace Rayfun
{

struct PakContents;
class Parameters;
class Resources;
class StateMachine;
class Logger;

class State : private sf::NonCopyable
{
    public:
        struct Context
        {
                sf::RenderWindow& window;
                StateMachine& stateMachine;
                Resources& resources;
                chaiscript::ChaiScript& scriptEngine;
                PakContents& mapPack;
                Parameters& params;
                Logger& logger;
        };

    public:
        explicit State(Context& t_context);

        virtual ~State() = default;

    public:
        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void handleEvent(const sf::Event& t_event) = 0;

        virtual void update(const sf::Time& t_deltaTime) = 0;

        virtual void display() = 0;

        virtual void initChai(){}

    protected:
        Context& m_context;
};

} // namespace Rayfun

#endif // STATE_HPP
