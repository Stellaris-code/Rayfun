/*  src/menustate.cpp MenuState - Yann BOUCHER (yann) 10/08/2016
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
#include "states/menustate.hpp"

#include "imgui/imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <chaiscript/chaiscript.hpp>

#include <Thor/Resources/SfmlLoaders.hpp>

#include "common.hpp"

#include "pakcontents.hpp"
#include "statemachine.hpp"
#include "constants.hpp"

namespace Rayfun
{

MenuState::MenuState(State::Context& t_context) :
    State(t_context)
{
    initChai();
    m_background = m_context.resources.textureHolder.acquire("menu/bckg", thor::Resources::fromFile<sf::Texture>("data/menu/bckg.png"),
                                                             thor::Resources::Reuse);
    m_bckgSprite.setTexture(m_background);
    loadMenuFile();
}

void MenuState::pause()
{

}

void MenuState::resume()
{

}

void MenuState::handleEvent(const sf::Event &)
{

}

void MenuState::update(const sf::Time &)
{
    if (m_drawCallback)
    {
        switch(m_drawCallback())
        {
            case SelectedOption::Play:
                loadLevel();
                break;
            case SelectedOption::Exit:
                m_context.stateMachine.popState();
                return;
            case SelectedOption::NoAction:
            default:
                ;
        }
    }
}

void MenuState::display()
{
    m_context.window.draw(m_bckgSprite);
}

void MenuState::initChai()
{
    m_context.scriptEngine.add_global_const(chaiscript::const_var(SelectedOption::Play), "Play");
    m_context.scriptEngine.add_global_const(chaiscript::const_var(SelectedOption::Exit), "Exit");
    m_context.scriptEngine.add_global_const(chaiscript::const_var(SelectedOption::NoAction), "NoAction");

    m_context.scriptEngine.add(chaiscript::fun([this]{ return m_context.mapPack.title; }), "mapPackTitle");
    m_context.scriptEngine.add(chaiscript::fun([this]{ return m_context.mapPack.date; }), "mapPackDate");
    m_context.scriptEngine.add(chaiscript::fun([this]{ return m_context.mapPack.desc; }), "mapPackDesc");
    m_context.scriptEngine.add(chaiscript::fun([this]{ return m_context.mapPack.author; }), "mapPackAuthor");
}

void MenuState::loadMenuFile()
{
    m_drawCallback = m_context.scriptEngine.eval_file<std::function<SelectedOption()>>(c_menuScript);
}

void MenuState::loadLevel()
{
    if (m_context.mapPack.level.intro)
    {
        m_context.stateMachine.pushState(StateID::Intro);
    }
    else
    {
        m_context.stateMachine.pushState(StateID::Game);
    }
}

}
