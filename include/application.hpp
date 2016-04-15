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

#include <SFML/System/Time.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <Thor/Resources.hpp>

#include <SelbaWard/BitmapFont.hpp>

#include "application_error.hpp"

#include "state.hpp"
#include "statefactory.hpp"

namespace Rayfun
{

class Application
{
    public:
        Application(const sf::VideoMode& t_videoMode, const std::string& t_windowName,
                    const sf::ContextSettings& t_contextSettings = sf::ContextSettings(), float t_fpsCap = 60);

    public:
        void changeState(StateID t_state);
        void pushState(StateID t_state);
        void popState();

        void run();

    private:
        void processEvents();
        void update(const sf::Time& t_deltatime);
        void updateStats(const sf::Time& t_deltatime);
        void render();

    private:
        sf::RenderWindow m_window {};

        thor::ResourceHolder<sf::Texture, std::string> m_textureHolder {};
        thor::ResourceHolder<sf::Font, std::string> m_fontHolder {};
        thor::ResourceHolder<selbaward::BitmapFont, std::string> m_bitmapFontHolder {};
        thor::ResourceHolder<sf::Sound, std::string> m_soundHolder {};
        thor::ResourceHolder<sf::Music, std::string> m_musicHolder {};

        std::stack<std::unique_ptr<State>> m_states;

        sf::Text m_statisticsText {};
        sf::Time m_statisticsUpdateTime {};
        std::size_t m_statisticsNumFrames { 0 };

        const sf::Time m_timePerFrame { sf::seconds(1.f / 60.f) };
};

} // namespace Rayfun

#endif // APPLICATION_HPP
