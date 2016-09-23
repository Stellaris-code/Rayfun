/*  src/hud/console.cpp Console - Yann BOUCHER (yann) 29/08/2016
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
#include "hud/console.hpp"

#include <cctype>
#include <iostream>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using boost::tokenizer;
using boost::escaped_list_separator;

namespace Rayfun
{

Console::Console(Logger& t_logger, const sf::Texture &t_tex, const sf::Vector2u &t_tileSize, unsigned t_numberOfTilesPerRow,
                 const sf::Vector2f &t_size) :
    m_textureTileSize(t_tileSize), m_logger(t_logger)
{
    setTexture(t_tex);
    setTileSize(t_tileSize);
    setNumberOfTilesPerRow(t_numberOfTilesPerRow);

    setSize(t_size);

    m_bckg.setFillColor(sf::Color(0, 0, 0, 200));
    m_bckg.setOutlineColor(sf::Color::Black);
    m_bckg.setOutlineThickness(1.5);

    m_inputLine.setFillColor(sf::Color(0, 0, 0, 150));
    m_inputLine.setOutlineColor(sf::Color::Black);
    m_inputLine.setOutlineThickness(1.5);

    m_cs.setThrowExceptions(true);
    m_cs.setUpdateAutomatically(false);
    m_cs.setShowCursor(false);
    m_cs.setShowBackground(false);
    m_cs.setScrollAutomatically(true);
    m_cs.setAddNewColorToPalette(true);
    m_cs.loadPalette(sw::ConsoleScreen::Palette::ColorsRgb);

    m_cs << Cs::Fg(0xFFFFFF);

    m_inputLine.setPosition(sf::Vector2f(0, m_cs.getSize().y + 100));
    m_inputText.setPosition(m_inputLine.getPosition());

    m_cursor.blinkRate = sf::seconds(0.5);

    setPosition(0, -m_cs.getSize().y - m_inputLine.getSize().y - 5);
}

Console::Console(Logger& t_logger) :
    m_logger(t_logger)
{
    m_bckg.setFillColor(sf::Color(0, 0, 0, 200));
    m_bckg.setOutlineColor(sf::Color::Black);
    m_bckg.setOutlineThickness(1.5);

    m_inputLine.setFillColor(sf::Color(0, 0, 0, 150));
    m_inputLine.setOutlineColor(sf::Color::Black);
    m_inputLine.setOutlineThickness(1.5);

    m_cs.setThrowExceptions(true);
    m_cs.setUpdateAutomatically(false);
    m_cs.setShowCursor(false);
    m_cs.setShowBackground(false);
    m_cs.setScrollAutomatically(true);
    m_cs.setAddNewColorToPalette(true);
    m_cs.loadPalette(sw::ConsoleScreen::Palette::ColorsRgb);

    m_cs << Cs::Fg(0xFFFFFF);

    m_inputLine.setPosition(sf::Vector2f(0, m_cs.getSize().y + 100));
    m_inputText.setPosition(m_inputLine.getPosition());

    m_cursor.blinkRate = sf::seconds(0.5);

    setPosition(0, -m_cs.getSize().y - m_inputLine.getSize().y - 5);
}

void Console::setTexture(const sf::Texture &t_tex)
{
    m_cs.setTexture(t_tex);
}

void Console::setTileSize(const sf::Vector2u &t_tileSize)
{
    m_cs.setTextureTileSize(t_tileSize);
    m_textureTileSize = t_tileSize;
}

void Console::setNumberOfTilesPerRow(unsigned t_num)
{
    m_cs.setNumberOfTextureTilesPerRow(t_num);
}

void Console::setSize(const sf::Vector2f &t_size)
{
    m_cs.setSize(t_size);
    m_cs.setMode(sf::Vector2u(t_size.x / m_textureTileSize.x,
                              t_size.y / m_textureTileSize.y));

    m_bckg.setSize(m_cs.getSize());

    m_inputLine.setSize(sf::Vector2f(m_cs.getSize().x, 20));
    m_inputLine.setPosition(sf::Vector2f(0, m_cs.getSize().y + 5));
    m_inputText.setPosition(m_inputLine.getPosition());

    m_cursor.setPosition(-m_inputLine.getPosition().x + m_cursor.thickness(), m_inputLine.getPosition().y);

    m_slideDown.setParams({ 0, -m_cs.getSize().y - m_inputLine.getSize().y - 5}, { 0, 0 });
    m_slideUp.setParams({ 0, 0 }, { 0, -m_cs.getSize().y - m_inputLine.getSize().y - 5 });

    setPosition(0, -m_cs.getSize().y - m_inputLine.getSize().y - 5);
}

void Console::setTextureOffset(const sf::Vector2u &t_offset)
{
    m_cs.setTextureOffset(t_offset);
}

void Console::setInputFont(const sf::Font &t_font)
{
    m_inputText.setFont(t_font);
    m_inputText.setCharacterSize(14);
    m_cursor.setHeight(18.6); // 14pt in pixels for a dpi of 96
}

void Console::setCallback(const std::string &t_command, const std::function<void (std::vector<std::string>)> &t_callback)
{
    m_callbacks[t_command] = t_callback;
}

void Console::setCvar(const std::string &t_cvar, const std::function<std::string()> &t_getter, const std::function<void(const std::string&)> &t_setter)
{
    m_cvars[t_cvar] = std::make_pair(t_getter, t_setter);
}

void Console::setCvar(const std::string &t_name, std::string &t_ref)
{
    m_cvars[t_name] = std::make_pair([&t_ref]{ return t_ref; }, [&t_ref](const std::string& val){ t_ref = val; });
}

void Console::show()
{
    if (!m_moving)
    {
        m_moving = true;
        m_animator.play() << "slideDown" << thor::Playback::notify([this]
        {
            m_visible = true;
            m_moving = false;
        });
    }
}

void Console::hide()
{
    if (!m_moving)
    {
        m_moving = true;
        m_animator.play() << "slideUp" << thor::Playback::notify([this]
        {
            m_visible = false;
            m_moving = false;
        });
    }
}

bool Console::visible() const
{
    return m_visible;
}

void Console::clear()
{
    m_cs.clear();
}

void Console::log(const std::string &t_str, Logger::Level t_level)
{
    switch (t_level)
    {
        case Logger::Level::Log:
            m_cs << Cs::Fg(0xFFFFFF);
            break;
        case Logger::Level::Warn:
            m_cs << Cs::Fg(0xFFFF00) << "[WARN] : ";
            break;
        case Logger::Level::Error:
            m_cs << Cs::Fg(0xFF0000) << "[ERROR] : ";
            break;
        case Logger::Level::Crit:
            m_cs << Cs::Fg(0xFF0000) << "[CRIT] : ";
            break;
    }
    m_cs << t_str << Cs::CursorCommand::Newline;
}

void Console::update(const sf::Time &t_dt)
{
    if (!m_animInitialized)
    {
        m_animInitialized = true;
        m_animationMap.addAnimation("slideDown", m_slideDown, sf::seconds(0.5));
        m_animationMap.addAnimation("slideUp", m_slideUp, sf::seconds(0.5));
    }

    m_runningTime += t_dt;

    m_cs.update();
    m_animator.update(t_dt);
    m_animator.animate(*this);

    m_cursor.update(t_dt);

    m_cs.setPosition(getPosition());
}

void Console::handleEvent(const sf::Event &t_event)
{
    bool resetMatches { false };

    if (t_event.type == sf::Event::TextEntered)
    {
        if ((std::iswgraph(t_event.text.unicode) || std::iswspace(t_event.text.unicode))
                && !std::iswcntrl(t_event.text.unicode)) // is it a printable char ?
        {
            insertString(t_event.text.unicode);
        }
    }

    if (t_event.type == sf::Event::KeyPressed)
    {
        resetMatches = true;

        switch (t_event.key.code)
        {
            case sf::Keyboard::BackSpace:
                if (!m_inputText.getString().isEmpty())
                {
                    removeCharAtCursor();
                }
                break;

            case sf::Keyboard::Delete:
                if (!m_inputText.getString().isEmpty())
                {
                    removeCharBehindCursor();
                }
                break;

            case sf::Keyboard::Return:
                if (!m_inputText.getString().isEmpty())
                {
                    std::string string = m_inputText.getString();
                    sendCommand(string);
                }
                break;

            case sf::Keyboard::Up:
                if (!m_commandHistory.empty())
                {
                    ++m_commandHistIdx;
                    if (m_commandHistIdx >= int(m_commandHistory.size()))
                    {
                        m_commandHistIdx = m_commandHistory.size() - 1;
                    }
                    setInputString(m_commandHistory[m_commandHistory.size() - m_commandHistIdx - 1]);
                }
                break;

            case sf::Keyboard::Down:
                if (!m_commandHistory.empty())
                {
                    --m_commandHistIdx;
                    if (m_commandHistIdx < 0)
                    {
                        m_commandHistIdx = -1;
                        setInputString("");
                    }
                    else
                    {
                        setInputString(m_commandHistory[m_commandHistory.size() - m_commandHistIdx - 1]);
                    }
                }
                break;

            case sf::Keyboard::Right:
                moveCursor(1);
                break;

            case sf::Keyboard::Left:
                moveCursor(-1);
                break;

            case sf::Keyboard::Tab:
                if (!m_inputText.getString().isEmpty())
                {
                    std::string match = findNextAutocomplete(boost::algorithm::trim_copy(m_inputText.getString().toAnsiString()));
                    if (!match.empty())
                    {
                        setInputString(match + " ");
                    }

                    resetMatches = false;
                }
                break;
        }
    }

    if (resetMatches)
    {
        resetCompletion();
    }
}

void Console::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_bckg, t_states);
    t_target.draw(m_cs, t_states);
    t_target.draw(m_inputLine, t_states);
    t_target.draw(m_inputText, t_states);
    t_target.draw(m_cursor, t_states);
}

void Console::updateBlink()
{
    if (int(m_runningTime.asSeconds()) % 2) // blink every second
    {
        m_cs.setShowCursor(true);
    }
    else
    {
        m_cs.setShowCursor(false);
    }
}

void Console::processCommand(const std::string& t_command, const std::vector<std::string> &t_args)
{
    m_callbacks.at(t_command)(t_args);
}

void Console::processCvar(const std::string &t_cvar, const std::string &t_arg)
{
    auto& cvar = m_cvars.at(t_cvar);
    if (!t_arg.empty())
    {
        cvar.second(t_arg);
    }
    m_logger.log(cvar.first());
}

bool Console::isCvar(const std::string &t_cvar)
{
    return m_cvars.find(t_cvar) != m_cvars.cend();
}

bool Console::isCommand(const std::string &t_command)
{
    return m_callbacks.find(t_command) != m_callbacks.cend();
}

void Console::sendCommand(const std::string &t_command)
{
    m_logger.log(t_command);
    m_commandHistIdx = -1;

    if (!m_commandHistory.empty())
    {
        // Do not push the same string !
        if (m_commandHistory.back() != t_command)
        {
            m_commandHistory.push_back(t_command);
        }
    }
    else
    {
        m_commandHistory.push_back(t_command);
    }

    std::vector<std::string> toks;

    tokenizer<escaped_list_separator<char>> tokenizer(t_command, escaped_list_separator<char>('\\', ' ', '\"'));

    for (const auto& token : tokenizer)
    {
        toks.push_back(token);
    }

    std::vector<std::string> args { toks.cbegin() + 1, toks.cend() };

    std::string args_str;
    for (const auto& s : args)
    {
        args_str += s + " ";
    }
    if (!args_str.empty())
    {
        args_str.pop_back(); // remove last space
    }

    std::string command = toks.front();
    if (isCommand(command))
    {
        processCommand(command, args);
    }
    else if (isCvar(command))
    {
        processCvar(command, args_str);
    }
    else
    {
        m_logger.log("'" + command + "' is neither a command or a cvar !", Logger::Warn);
    }
    setInputString("");
    resetCompletion();
}

void Console::setInputString(const sf::String &t_command, bool t_moveCursor)
{
    m_inputText.setString(t_command);

    std::vector<std::string> toks;
    std::string stdStr = t_command;
    boost::split(toks, stdStr, boost::is_space());

    if (!toks.empty())
    {
        std::string firstTok = toks.front();

        if (!isCvar(firstTok) && !isCommand(firstTok)) // not a valid command
        {
            m_inputText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            m_inputText.setFillColor(sf::Color::Green);
        }

    }
    if (t_moveCursor)
    {
        moveCursorToEnd();
    }
}

void Console::insertString(const sf::String &t_str)
{
    std::string str = m_inputText.getString();
    str.insert(m_cursorPos, t_str);
    setInputString(str, false);
    moveCursor(+1);
}

void Console::removeCharAtCursor()
{
    if (m_cursorPos > 0)
    {
        moveCursor(-1);
        std::string str = m_inputText.getString();
        str.erase(m_cursorPos, 1);
        setInputString(str, false);
    }
}

void Console::removeCharBehindCursor()
{
    std::string str = m_inputText.getString();
    str.erase(m_cursorPos, 1);
    setInputString(str, false);
}

void Console::moveCursor(int step)
{
    if (m_cursorPos + step >= int(m_inputText.getString().getSize()))
    {
        step = m_inputText.getString().getSize() - (m_cursorPos + step) + 1;
    }
    if (m_cursorPos + step < 0)
    {
        step = std::abs(m_cursorPos + step) - 1;
    }
    m_cursorPos += step;

    m_cursor.move((14/2 + m_cursor.thickness()/2) * step, 0);

    m_cursor.resetBlink();
}

void Console::moveCursorToEnd()
{
    m_cursorPos = m_inputText.getString().getSize();
    m_cursor.setPosition(m_inputLine.getPosition().x + m_cursor.thickness() + m_inputText.getLocalBounds().width - 1,
                         m_inputLine.getPosition().y);
}

int Console::characterWidth(sf::Uint32 codePoint) const
{
    return m_inputText.getFont()->getGlyph(codePoint, 14, false).bounds.width;
}

std::string Console::findNextAutocomplete(const std::string& t_command)
{
    std::string match;
    std::string command;

    if (m_matchedCommands.empty() && m_commandToMatch.empty())
    {
        command = t_command;
        m_commandToMatch = command;
    }
    else
    {
        command = m_commandToMatch;
    }

    {
        // Try to find matching command
        auto iter = std::find_if(m_callbacks.cbegin(), m_callbacks.cend(),
                                 [command, this](const decltype(m_callbacks)::value_type& t_pair)
        {
            return t_pair.first.substr(0, command.size()) == command &&
                    std::find(m_matchedCommands.cbegin(), m_matchedCommands.cend(), t_pair.first)
                    == m_matchedCommands.cend(); // Check not already matched
        });

        if (iter != m_callbacks.cend())
        {
            match = iter->first;
            m_matchedCommands.push_back(match);
        }
    }

    // If no command found try to search in cvars
    if (match.empty())
    {
        auto iter = std::find_if(m_cvars.cbegin(), m_cvars.cend(),
                                 [command, this](const cvars_t::value_type& t_pair)
        {
            return t_pair.first.substr(0, command.size()) == command &&
                    std::find(m_matchedCommands.cbegin(), m_matchedCommands.cend(), t_pair.first)
                    == m_matchedCommands.cend(); // Check not already matched
        });

        if (iter != m_cvars.cend())
        {
            match = iter->first;
            m_matchedCommands.push_back(match);
        }
        else if (!m_matchedCommands.empty()) // cycle matches
        {
            m_matchedCommands.clear();
            match = findNextAutocomplete(command);
        }
    }

    return match;
}

void Console::resetCompletion()
{
    m_matchedCommands.clear();
    m_commandToMatch.clear();
}


}
