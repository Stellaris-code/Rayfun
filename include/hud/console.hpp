/*  include/hud/console.hpp Console - Yann BOUCHER (yann) 29/08/2016
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
#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/Window/Event.hpp>

#include <SelbaWard/ConsoleScreen.hpp>

#include <Thor/Animations.hpp>

#include "logger.hpp"
#include "animations/moveanimation.hpp"
#include "hud/textcursor.hpp"

namespace Rayfun
{

class Console : public sf::Drawable, public sf::Transformable
{
    private:
        using cvars_t = std::map<std::string, std::pair<std::function<std::string()>, std::function<void(const std::string&)>>>;

    public:
        Console(Logger& t_logger, const sf::Texture &t_tex, const sf::Vector2u& t_tileSize, unsigned t_numberOfTilesPerRow,
                const sf::Vector2f &t_size);
        Console(Logger& t_logger);

    public:
        void setTexture(const sf::Texture& t_tex);
        void setTileSize(const sf::Vector2u& t_tileSize);
        void setNumberOfTilesPerRow(unsigned t_num);
        void setSize(const sf::Vector2f& t_size);
        void setTextureOffset(const sf::Vector2u& t_offset);
        void setInputFont(const sf::Font& t_font);

        void setCallback(const std::string& t_command,
                         const std::function<void(std::vector<std::string>)>& t_callback);

        void setCvar(const std::string& t_cvar, const std::function<std::string()>& t_getter,
                     const std::function<void(const std::string &)> &t_setter);
        void setCvar(const std::string& t_name, std::string& t_ref);

        void show();
        void hide();

        bool visible() const;

        void clear();

        void log(const std::string& t_str, Logger::Level t_level = Logger::Level::Log);

        void update(const sf::Time& t_dt);
        void handleEvent(const sf::Event& t_event);

        void sendCommand(const std::string& t_command);

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        void updateBlink();
        void processCommand(const std::string &t_command, const std::vector<std::string>& t_args);
        void processCvar(const std::string& t_cvar, const std::string& t_arg);

        bool isCvar(const std::string& t_cvar);
        bool isCommand(const std::string& t_command);

        void setInputString(const sf::String& t_command, bool t_moveCursor = true);
        void insertString(const sf::String& t_str);
        void removeCharAtCursor();
        void removeCharBehindCursor();
        void moveCursor(int step);
        void moveCursorToEnd();

        int characterWidth(sf::Uint32 codePoint) const;

        std::string findNextAutocomplete(const std::string &t_command);
        void resetCompletion();

    private:
        sw::ConsoleScreen m_cs;
        sf::RectangleShape m_bckg;
        sf::RectangleShape m_inputLine;
        sf::Text m_inputText;
        sf::Vector2u m_textureTileSize;
        thor::AnimationMap<sf::Transformable&, std::string> m_animationMap;
        thor::Animator<sf::Transformable&, std::string> m_animator { m_animationMap };

        TextCursor m_cursor;
        int m_cursorPos { 0 };

        cvars_t m_cvars;

        Logger& m_logger;

        bool m_animInitialized { false };
        bool m_visible { false };
        bool m_moving { false };

        std::map<std::string, std::function<void(std::vector<std::string>)>> m_callbacks;
        std::vector<std::string> m_commandHistory;
        std::string m_commandToMatch;
        int m_commandHistIdx { -1 };
        std::vector<std::string> m_matchedCommands;

        sf::Time m_runningTime { sf::Time::Zero };

        MoveAnimation m_slideDown { {0, -m_cs.getSize().y}, {0, 0} };
        MoveAnimation m_slideUp { {0, 0}, {0, -m_cs.getSize().y} };

};

}

#endif // CONSOLE_HPP
