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

#include <chaiscript/chaiscript_stdlib.hpp>

#include <SFML/Window/Event.hpp>

#include "constants.hpp"
#include "loaders/configreader.hpp"
#include "loaders/bindingreader.hpp"
#include "loaders/pakreader.hpp"
#include "chaiinterfaceloader.hpp"

namespace Rayfun
{

Application::Application(const sf::VideoMode &t_videoMode, const std::string &t_windowName,
                         const std::string& t_pakPath,
                         const sf::ContextSettings &t_contextSettings, unsigned t_fpsCap)
    : m_window(t_videoMode, t_windowName, sf::Style::Default, t_contextSettings),
      m_scriptEngine(chaiscript::Std_Lib::library()), m_timePerFrame(sf::seconds(1.f / t_fpsCap))
{
    m_window.setFramerateLimit(t_fpsCap);
    loadConfig();
    loadPak(t_pakPath);

    loadChaiInterfaces(m_scriptEngine, m_resourcesObject);

    m_fontHolder.acquire("ubuntu_mono", thor::Resources::fromFile<sf::Font>("data/fonts/UbuntuMono-R.ttf"));
    m_fontHolder.acquire("doom", thor::Resources::fromFile<sf::Font>("data/fonts/DooM.ttf"));
    m_imageHolder.acquire("decals/bullet", thor::Resources::fromFile<sf::Image>("data/decals/bullet.png"));
    m_imageHolder.acquire("marine", thor::Resources::fromFile<sf::Image>("data/marinesheet.png"));
    m_textureHolder.acquire("hud/minimap/tileset", thor::Resources::fromFile<sf::Texture>("data/hud/minimap_tileset.png"));
    m_textureHolder.acquire("hud/minimap/arrow", thor::Resources::fromFile<sf::Texture>("data/hud/minimap_arrow.png"));
    m_textureHolder.acquire("spritesheets/doom_weapons", thor::Resources::fromFile<sf::Texture>("data/doomWeapons.png"));
    m_imageHolder.acquire("sprites/pillar", thor::Resources::fromFile<sf::Image>("data/pillar.png"));
    m_imageHolder.acquire("spritesheets/doom_items", thor::Resources::fromFile<sf::Image>("data/doomSpriteSheet.png"));
    m_soundHolder.acquire("weaponclick", thor::Resources::fromFile<sf::SoundBuffer>("data/sound/weaponclick.wav"));
    m_statisticsText.setFont(m_fontHolder["ubuntu_mono"]);
    m_statisticsText.setColor(sf::Color::White);
    m_statisticsText.setPosition(5.f, 5.f);
    m_statisticsText.setCharacterSize(10);

    pushState(StateID::Game);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        updateStats(elapsedTime);
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(m_timePerFrame);

            if (m_states.empty())
            {
                m_window.close();
            }
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

    m_states.push(makeState(t_state, m_contextObject));
}

void Application::pushState(Rayfun::StateID t_state)
{
    m_states.push(makeState(t_state, m_contextObject));
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
    m_params.bindings.clearEvents();
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_params.bindings.pushEvent(event);
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

void Application::loadConfig()
{
    m_params = Config::parseIntoParameters(c_configFile);
    loadBindings();
}

void Application::loadBindings()
{
    m_params.bindings = Binding::jsonToActionMap(c_bindingFile, m_bindingsDef);
}

void Application::loadPak(const std::string &t_path)
{
    PakReader reader(m_scriptEngine, m_contextObject);
    reader.open(t_path, m_pakContents);
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
