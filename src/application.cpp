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
#include <SFML/System/Err.hpp>

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
      m_imgui(m_window), m_statemachine(m_contextObject, m_window),
      m_scriptEngine(chaiscript::Std_Lib::library()),
      m_timePerFrame(sf::seconds(1.f / t_fpsCap))
{
    glewInit();

    m_window.setFramerateLimit(t_fpsCap);
    m_window.setKeyRepeatEnabled(false);
    loadConfig();
    loadPak(t_pakPath);

    loadChaiInterfaces(m_scriptEngine, m_resourcesObject);
    initChai();
    initCommands();

    m_fontHolder.acquire("ubuntu_mono", thor::Resources::fromFile<sf::Font>("data/fonts/UbuntuMono-R.ttf"));
    m_fontHolder.acquire("doom", thor::Resources::fromFile<sf::Font>("data/fonts/DooM.ttf"));
    m_fontHolder.acquire("gohu", thor::Resources::fromFile<sf::Font>("data/fonts/GohuFont-Medium.ttf"));
    m_imageHolder.acquire("decals/bullet", thor::Resources::fromFile<sf::Image>("data/decals/bullet.png"));
    m_imageHolder.acquire("marine", thor::Resources::fromFile<sf::Image>("data/marinesheet.png"));
    m_textureHolder.acquire("hud/minimap/tileset", thor::Resources::fromFile<sf::Texture>("data/hud/minimap_tileset.png"));
    m_textureHolder.acquire("hud/minimap/arrow", thor::Resources::fromFile<sf::Texture>("data/hud/minimap_arrow.png"));
    m_textureHolder.acquire("hud/medal", thor::Resources::fromFile<sf::Texture>("data/hud/medal.png"));
    m_textureHolder.acquire("spritesheets/doom_weapons", thor::Resources::fromFile<sf::Texture>("data/doomWeapons.png"));
    m_textureHolder.acquire("textures/stone", thor::Resources::fromFile<sf::Texture>("data/greystone.png"));
    m_textureHolder.acquire("fonts/bitmap/gohu", thor::Resources::fromFile<sf::Texture>("data/fonts/bitmap/gohu.png"));
    m_textureHolder.acquire("maps/map", thor::Resources::fromFile<sf::Texture>("data/map.png"));
    m_imageHolder.acquire("sprites/pillar", thor::Resources::fromFile<sf::Image>("data/pillar.png"));
    m_imageHolder.acquire("spritesheets/doom_items", thor::Resources::fromFile<sf::Image>("data/doomSpriteSheet.png"));
    m_soundHolder.acquire("weaponclick", thor::Resources::fromFile<sf::SoundBuffer>("data/sound/weaponclick.wav"));

    m_statisticsText.setFont(m_fontHolder["ubuntu_mono"]);
    m_statisticsText.setFillColor(sf::Color::White);
    m_statisticsText.setOutlineColor(sf::Color::Black);
    m_statisticsText.setOutlineThickness(0.5);
    m_statisticsText.setPosition(m_window.getSize().x - 5.f - m_statisticsText.getLocalBounds().width, 5.f);
    m_statisticsText.setCharacterSize(10);

    m_console.setTexture(m_textureHolder["fonts/bitmap/gohu"]);
    m_console.setTileSize({8, 14});
    m_console.setNumberOfTilesPerRow(32);
    m_console.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y / 2.f));
    m_console.setInputFont(m_fontHolder["gohu"]);

    m_logger.setErrOstream(&std::cerr);
    m_logger.setLogOstream(&std::cout);
    m_logger.setConsole(&m_console);

    m_statemachine.pushState(StateID::Menu);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen())
    {
reloop:
        sf::Time elapsedTime = clock.restart();
        updateStats(elapsedTime);
        timeSinceLastUpdate += elapsedTime;
        do
        {
            if (m_statemachine.changed())
            {
                if (!m_firstScreen)
                {
                    m_melting = true;
                    m_screenMelt.reset(m_statemachine.lastStateFrame());
                }
                else
                {
                    m_firstScreen = false;
                }
                goto reloop;
            }

            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(m_timePerFrame);

            if (m_statemachine.empty())
            {
                m_window.close();
                return;
            }
        } while (timeSinceLastUpdate > m_timePerFrame);
        render();
    }
}

void Application::processEvents()
{
    std::setlocale(LC_ALL, ""); // enable UTF-8 !
    m_params.bindings.clearEvents();
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_params.bindings.pushEvent(event);
        if (!m_console.visible())
        {
            ImGui::SFML::ProcessEvent(event);
            m_statemachine.top()->handleEvent(event);
        }
        else
        {
            m_console.handleEvent(event);
        }
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;

            default:
                break;
        }
    }

    if (m_params.bindings.isActive("toggleconsole"))
    {
        if (m_console.visible())
        {
            m_console.hide();
        }
        else
        {
            m_console.show();
        }
    }

    if (m_params.bindings.isActive("quit"))
    {
        m_window.close();
    }

    std::setlocale(LC_ALL, "C"); // restore locale
}

void Application::update(const sf::Time &t_deltatime)
{
    if (!m_console.visible())
    {
        ImGui::SFML::Update(t_deltatime);
        m_statemachine.top()->update(t_deltatime);
    }
    m_console.update(t_deltatime);

    if (m_melting)
    {
        m_screenMelt.update(t_deltatime);
    }

    if (m_screenMelt.completed())
    {
        m_melting = false;
    }
}

void Application::render()
{
    m_window.clear();
    m_statemachine.top()->display();
    ImGui::Render();
    if (m_melting)
    {
        m_window.draw(m_screenMelt);
    }
    m_window.draw(m_console);
    m_window.draw(m_statisticsText);
    m_window.display();
}

void Application::initChai()
{
    m_scriptEngine.add(chaiscript::fun([this]{ return m_window.getSize(); }), "displaySize");
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
        m_statisticsText.setPosition(m_window.getSize().x - 5.f - m_statisticsText.getLocalBounds().width, 5.f);
        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}

} // namespace Rayfun
