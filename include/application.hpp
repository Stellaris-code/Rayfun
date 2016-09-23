/*  include/application.hpp Application - Yann BOUCHER (yann) 15/04/2016
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
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <stack>
#include <string>

#include <chaiscript/chaiscript.hpp>

#include <GL/glew.h>

#include <SFML/System/Time.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <Thor/Resources.hpp>

#include <SelbaWard/BitmapFont.hpp>

#include <imgui/imgui.h>

#include "common.hpp"

#include "state.hpp"
#include "statemachine.hpp"
#include "parameters.hpp"
#include "pakcontents.hpp"
#include "screenmelt.hpp"
#include "imgui-wrapper.hpp"
#include "logger.hpp"
#include "hud/console.hpp"

namespace Rayfun
{

class application_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_application_error(const std::string& t_why)
{
    throw application_error("Application error : " + t_why);
}

class Application : private sf::NonCopyable
{

    public:
        Application(const sf::VideoMode& t_videoMode, const std::string& t_windowName,
                    const std::string& t_pakPath,
                    const sf::ContextSettings& t_contextSettings = sf::ContextSettings(),
                    unsigned t_fpsCap = 60);

    public:
        void run();

    private:
        void processEvents();
        void update(const sf::Time& t_deltatime);
        void updateStats(const sf::Time& t_deltatime);
        void render();

    private:
        void initChai();
        void initCommands();
        void loadConfig();
        void loadBindings();
        void loadPak(const std::string& t_path);

    private:
        std::map<std::string, thor::Action::ActionType> m_bindingsDef
        {
            { "fire", thor::Action::PressOnce},
            { "use", thor::Action::PressOnce},
            { "toggleconsole", thor::Action::PressOnce},
            { "quit", thor::Action::PressOnce},
            { "minimap", thor::Action::Hold},
            { "moveforward", thor::Action::Hold},
            { "movebackward", thor::Action::Hold},
            { "turnleft", thor::Action::Hold},
            { "turnright", thor::Action::Hold},
            { "strafeleft", thor::Action::Hold},
            { "straferight", thor::Action::Hold}
        };

    private:
        sf::RenderWindow m_window {};

        Utility::IMGUIWrapper m_imgui {m_window};

        thor::ResourceHolder<sf::Texture, std::string> m_textureHolder {};
        thor::ResourceHolder<sf::Image, std::string> m_imageHolder {};
        thor::ResourceHolder<sf::Font, std::string> m_fontHolder {};
        thor::ResourceHolder<selbaward::BitmapFont, std::string> m_bitmapFontHolder {};
        thor::ResourceHolder<sf::SoundBuffer, std::string> m_soundHolder {};

        StateMachine m_statemachine;
        MeltTransition m_screenMelt;
        bool m_melting { false };
        bool m_firstScreen { true };

        chaiscript::ChaiScript m_scriptEngine;
        Parameters m_params;

        PakContents m_pakContents;

        Logger m_logger;

        Resources m_resourcesObject { m_textureHolder, m_imageHolder, m_fontHolder,
                    m_bitmapFontHolder, m_soundHolder };

        State::Context m_contextObject { m_window, m_statemachine,
                    m_resourcesObject,
                    m_scriptEngine,
                    m_pakContents, m_params,
                    m_logger};

        Console m_console { m_logger };

        sf::Text m_statisticsText {};
        sf::Time m_statisticsUpdateTime {};
        std::size_t m_statisticsNumFrames { 0 };

        const sf::Time m_timePerFrame { sf::seconds(1.f / 60.f) };
};

} // namespace Rayfun

#endif // APPLICATION_HPP
