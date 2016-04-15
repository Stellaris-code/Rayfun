/*  src/application.cpp Application - Yann BOUCHER (yann) 15/04/2016
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
#include "application.hpp"

namespace Rayfun
{

Application::Application(const sf::VideoMode &t_videoMode, const std::string &t_windowName,
                         const sf::ContextSettings &t_contextSettings, float t_fpsCap)
    : m_window(t_videoMode, t_windowName, sf::Style::Default, t_contextSettings),
      m_timePerFrame(sf::seconds(1.f / t_fpsCap))
{
    m_fontHolder.acquire("UbuntuMono", thor::Resources::fromFile<sf::Font>("data/fonts/UbuntuMono-R.ttf"));
    m_statisticsText.setFont(m_fontHolder["UbuntuMono"]);
    m_statisticsText.setPosition(5.f, 5.f);
    m_statisticsText.setCharacterSize(10);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(elapsedTime);
        }
        render();
    }
}

void Application::changeState(Rayfun::StateID t_state)
{
    if (!m_states.empty())
    {
        m_states.pop();
    }

    m_states.push(makeState(t_state));
}

void Application::pushState(Rayfun::StateID t_state)
{
    m_states.push(makeState(t_state));
}

void Application::popState()
{
    if (m_states.empty())
    {
        throw_application_error("Tried to pop empty state stack !");
    }

    m_states.pop();
}

void Application::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_states.top()->handleEvent(event);
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;

            default:
                break;
        }
    }
}

void Application::update(const sf::Time &t_deltatime)
{
    m_states.top()->update(t_deltatime);
}

void Application::render()
{
    m_window.clear();
    m_states.top()->display();
    m_window.draw(m_statisticsText);
    m_window.display();
}

void Application::updateStats(const sf::Time& t_deltaTime)
{
    m_statisticsUpdateTime += t_deltaTime;

    m_statisticsNumFrames += 1;
    if (m_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_statisticsText.setString(
                    "Frames / Second = " + std::to_string(m_statisticsNumFrames)
                    + "\nTime / Update = " + std::to_string(
                        static_cast<std::size_t>
                        (m_statisticsUpdateTime.asMicroseconds()) / m_statisticsNumFrames)
                    + "us");
        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}

} // namespace Rayfun
