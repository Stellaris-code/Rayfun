/*  src/states/introstate.cpp IntroState - Yann BOUCHER (yann) 11/08/2016
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
#include "states/introstate.hpp"

#include <imgui/imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

#include "common.hpp"
#include "parameters.hpp"
#include "statemachine.hpp"
#include "pakcontents.hpp"

namespace Rayfun
{

IntroState::IntroState(State::Context &t_context)
    : State(t_context)
{
    m_bckgSprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(m_context.window.getSize())));
    m_context.mapPack.level.beginBkg.setRepeated(true);
    m_bckgSprite.setTexture(m_context.mapPack.level.beginBkg);
}

void IntroState::pause()
{

}

void IntroState::resume()
{

}

void IntroState::handleEvent(const sf::Event &)
{
    if (m_context.params.bindings.isActive("use") || m_context.params.bindings.isActive("fire"))
    {
        m_context.stateMachine.changeState(StateID::Game);
    }
}

void IntroState::update(const sf::Time &)
{

}

void IntroState::display()
{
    m_context.window.draw(m_bckgSprite);

    drawImgui();
}

void IntroState::drawImgui()
{
    ImGui::PushStyleVar(ImGuiStyleVar_TextOutline, 1);

    ImGui::SetNextWindowPosCenter();
    ImGui::SetNextWindowSize(ImVec2(m_context.window.getSize().x, (m_context.window.getSize().y)));
    ImGui::SetNextWindowContentSize(ImVec2(m_context.window.getSize().x, (m_context.window.getSize().y)));

    ImGui::Begin(m_context.mapPack.level.name.c_str(),
            nullptr, ImVec2(0, 0), 0,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::SetWindowFontScale(0.5);

    ImGui::TextWrapped("%s", m_context.mapPack.level.beginMessage.c_str());

    ImGui::SetWindowFontScale(1);
    ImGui::End();

    ImGui::PopStyleVar();
}

}
