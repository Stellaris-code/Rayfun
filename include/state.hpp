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

#include <SFML/System/Time.hpp>

#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include <Thor/Resources.hpp>

#include <SelbaWard/BitmapFont.hpp>

namespace Rayfun
{

class State
{
    public:
        struct Context
        {
                sf::RenderWindow& window;
                thor::ResourceHolder<sf::Texture, std::string>& m_textureHolder;
                thor::ResourceHolder<sf::Font, std::string>& m_fontHolder;
                thor::ResourceHolder<selbaward::BitmapFont, std::string>& m_bitmapFontHolder;
                thor::ResourceHolder<sf::Sound, std::string>& m_soundHolder;
                thor::ResourceHolder<sf::Music, std::string>& m_musicHolder;
        };

    public:
        explicit State(Context t_context);

        virtual ~State() = default;

    public:
        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void handleEvent(const sf::Event& t_event) = 0;

        virtual void update(const sf::Time& t_deltaTime) = 0;

        virtual void display() = 0;

    private:
        Context m_context;
};

} // namespace Rayfun

#endif // STATE_HPP
