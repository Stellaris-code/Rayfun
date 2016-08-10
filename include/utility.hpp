/*  utility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 12/04/2016
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
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <fstream>
#include <stdexcept>

#include <iostream>

#include <boost/algorithm/string.hpp>

#include <json/value.h>
#include <json/reader.h>

#include <SFML/System/Vector2.hpp>

#include <Thor/Input/Action.hpp>

#include "common.hpp"

namespace Rayfun
{

namespace Utility
{

template <typename T>
class LookupTable
{
    public:
        LookupTable(size_t t_size, const std::function<T(size_t)>& t_generator)
            : m_generator(t_generator)
        {
            m_lut.resize(t_size);
            generate();
        }

    public:
        void generate()
        {
            size_t n { 0 };
            std::generate(m_lut.begin(), m_lut.end(), [&n, this]{
                return m_generator(n++);
            });
        }

        operator std::vector<T>() const { return m_lut; }
        const T& operator[](size_t i) const { return m_lut[i]; }

    private:
        std::vector<T> m_lut;
        std::function<T(size_t)> m_generator;
};

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}

inline Side oppositeSide(Side t_side)
{
    switch (t_side)
    {
        case Side::North:
            return Side::South;
        case Side::South:
            return Side::North;
        case Side::East:
            return Side::West;
        case Side::West:
            return Side::East;
    }
}

inline sf::Vector2i sideToVector(Side t_side)
{
    switch (t_side)
    {
        case Side::North:
            return {0, 1};
        case Side::South:
            return {0, 1};
        case Side::East:
            return {1, 0};
        case Side::West:
            return {1, 0};
    }
}

inline std::string filenameFromPath(const std::string& t_path)
{
    std::vector<std::string> toks;
    boost::split(toks, t_path, boost::is_any_of("/\\"));
    return toks.back();
}

inline Json::Value fileToJsonTree(const std::string& t_path)
{
    std::ifstream fstream(t_path);
    if (!fstream)
    {
        throw std::runtime_error("Cannot open " + t_path + " !");
    }
    try
    {
        Json::Value root;
        fstream >> root;
        return root;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Error parsing json file '" + t_path + "' : " + e.what());
    }
}

inline thor::Action stringToAction(const std::string& t_str, thor::Action::ActionType t_actionType)
{
    std::string str = t_str;
    boost::erase_all(str, " ");
    boost::to_lower(str);

    if (str == "a")
    {
        return thor::Action(sf::Keyboard::Key::A, t_actionType);
    }
    else if (str == "b")
    {
        return thor::Action(sf::Keyboard::Key::B, t_actionType);
    }
    else if (str == "c")
    {
        return thor::Action(sf::Keyboard::Key::C, t_actionType);
    }
    else if (str == "d")
    {
        return thor::Action(sf::Keyboard::Key::D, t_actionType);
    }
    else if (str == "e")
    {
        return thor::Action(sf::Keyboard::Key::E, t_actionType);
    }
    else if (str == "f")
    {
        return thor::Action(sf::Keyboard::Key::F, t_actionType);
    }
    else if (str == "g")
    {
        return thor::Action(sf::Keyboard::Key::G, t_actionType);
    }
    else if (str == "h")
    {
        return thor::Action(sf::Keyboard::Key::H, t_actionType);
    }
    else if (str == "i")
    {
        return thor::Action(sf::Keyboard::Key::I, t_actionType);
    }
    else if (str == "j")
    {
        return thor::Action(sf::Keyboard::Key::J, t_actionType);
    }
    else if (str == "k")
    {
        return thor::Action(sf::Keyboard::Key::K, t_actionType);
    }
    else if (str == "l")
    {
        return thor::Action(sf::Keyboard::Key::L, t_actionType);
    }
    else if (str == "m")
    {
        return thor::Action(sf::Keyboard::Key::M, t_actionType);
    }
    else if (str == "n")
    {
        return thor::Action(sf::Keyboard::Key::N, t_actionType);
    }
    else if (str == "o")
    {
        return thor::Action(sf::Keyboard::Key::O, t_actionType);
    }
    else if (str == "p")
    {
        return thor::Action(sf::Keyboard::Key::P, t_actionType);
    }
    else if (str == "q")
    {
        return thor::Action(sf::Keyboard::Key::Q, t_actionType);
    }
    else if (str == "r")
    {
        return thor::Action(sf::Keyboard::Key::R, t_actionType);
    }
    else if (str == "s")
    {
        return thor::Action(sf::Keyboard::Key::S, t_actionType);
    }
    else if (str == "t")
    {
        return thor::Action(sf::Keyboard::Key::T, t_actionType);
    }
    else if (str == "u")
    {
        return thor::Action(sf::Keyboard::Key::U, t_actionType);
    }
    else if (str == "v")
    {
        return thor::Action(sf::Keyboard::Key::V, t_actionType);
    }
    else if (str == "w")
    {
        return thor::Action(sf::Keyboard::Key::W, t_actionType);
    }
    else if (str == "x")
    {
        return thor::Action(sf::Keyboard::Key::X, t_actionType);
    }
    else if (str == "y")
    {
        return thor::Action(sf::Keyboard::Key::Y, t_actionType);
    }
    else if (str == "z")
    {
        return thor::Action(sf::Keyboard::Key::Z, t_actionType);
    }
    else if (str == "0")
    {
        return thor::Action(sf::Keyboard::Key::Num0, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad0, t_actionType);
    }
    else if (str == "1")
    {
        return thor::Action(sf::Keyboard::Key::Num1, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad1, t_actionType);
    }
    else if (str == "2")
    {
        return thor::Action(sf::Keyboard::Key::Num2, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad2, t_actionType);
    }
    else if (str == "3")
    {
        return thor::Action(sf::Keyboard::Key::Num3, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad3, t_actionType);
    }
    else if (str == "4")
    {
        return thor::Action(sf::Keyboard::Key::Num4, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad4, t_actionType);
    }
    else if (str == "5")
    {
        return thor::Action(sf::Keyboard::Key::Num5, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad5, t_actionType);
    }
    else if (str == "6")
    {
        return thor::Action(sf::Keyboard::Key::Num6, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad6, t_actionType);
    }
    else if (str == "7")
    {
        return thor::Action(sf::Keyboard::Key::Num7, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad7, t_actionType);
    }
    else if (str == "8")
    {
        return thor::Action(sf::Keyboard::Key::Num8, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad8, t_actionType);
    }
    else if (str == "9")
    {
        return thor::Action(sf::Keyboard::Key::Num9, t_actionType) ||
                thor::Action(sf::Keyboard::Key::Numpad9, t_actionType);
    }
    else if (str == "escape")
    {
        return thor::Action(sf::Keyboard::Key::Escape, t_actionType);
    }
    else if (str == "lcontrol")
    {
        return thor::Action(sf::Keyboard::Key::LControl, t_actionType);
    }
    else if (str == "rcontrol")
    {
        return thor::Action(sf::Keyboard::Key::RControl, t_actionType);
    }
    else if (str == "lshift")
    {
        return thor::Action(sf::Keyboard::Key::LShift, t_actionType);
    }
    else if (str == "rshift")
    {
        return thor::Action(sf::Keyboard::Key::RShift, t_actionType);
    }
    else if (str == "lalt")
    {
        return thor::Action(sf::Keyboard::Key::LAlt, t_actionType);
    }
    else if (str == "ralt")
    {
        return thor::Action(sf::Keyboard::Key::RAlt, t_actionType);
    }
    else if (str == "lsystem")
    {
        return thor::Action(sf::Keyboard::Key::LSystem, t_actionType);
    }
    else if (str == "rsystem")
    {
        return thor::Action(sf::Keyboard::Key::RSystem, t_actionType);
    }
    else if (str == "menu")
    {
        return thor::Action(sf::Keyboard::Key::Menu, t_actionType);
    }
    else if (str == "[")
    {
        return thor::Action(sf::Keyboard::Key::LBracket, t_actionType);
    }
    else if (str == "]")
    {
        return thor::Action(sf::Keyboard::Key::RBracket, t_actionType);
    }
    else if (str == "semicolon")
    {
        return thor::Action(sf::Keyboard::Key::SemiColon, t_actionType);
    }
    else if (str == "comma")
    {
        return thor::Action(sf::Keyboard::Key::Comma, t_actionType);
    }
    else if (str == "period")
    {
        return thor::Action(sf::Keyboard::Key::Period, t_actionType);
    }
    else if (str == "quote")
    {
        return thor::Action(sf::Keyboard::Key::Quote, t_actionType);
    }
    else if (str == "slash")
    {
        return thor::Action(sf::Keyboard::Key::Slash, t_actionType);
    }
    else if (str == "backslash")
    {
        return thor::Action(sf::Keyboard::Key::BackSlash, t_actionType);
    }
    else if (str == "tilde")
    {
        return thor::Action(sf::Keyboard::Key::Tilde, t_actionType);
    }
    else if (str == "equal")
    {
        return thor::Action(sf::Keyboard::Key::Equal, t_actionType);
    }
    else if (str == "dash")
    {
        return thor::Action(sf::Keyboard::Key::Dash, t_actionType);
    }
    else if (str == "space")
    {
        return thor::Action(sf::Keyboard::Key::Space, t_actionType);
    }
    else if (str == "return")
    {
        return thor::Action(sf::Keyboard::Key::Return, t_actionType);
    }
    else if (str == "backspace")
    {
        return thor::Action(sf::Keyboard::Key::BackSpace, t_actionType);
    }
    else if (str == "tab")
    {
        return thor::Action(sf::Keyboard::Key::Tab, t_actionType);
    }
    else if (str == "pageup")
    {
        return thor::Action(sf::Keyboard::Key::PageUp, t_actionType);
    }
    else if (str == "pagedown")
    {
        return thor::Action(sf::Keyboard::Key::PageDown, t_actionType);
    }
    else if (str == "end")
    {
        return thor::Action(sf::Keyboard::Key::End, t_actionType);
    }
    else if (str == "home")
    {
        return thor::Action(sf::Keyboard::Key::Home, t_actionType);
    }
    else if (str == "insert")
    {
        return thor::Action(sf::Keyboard::Key::Insert, t_actionType);
    }
    else if (str == "delete")
    {
        return thor::Action(sf::Keyboard::Key::Delete, t_actionType);
    }
    else if (str == "+")
    {
        return thor::Action(sf::Keyboard::Key::Add, t_actionType);
    }
    else if (str == "-")
    {
        return thor::Action(sf::Keyboard::Key::Subtract, t_actionType);
    }
    else if (str == "*")
    {
        return thor::Action(sf::Keyboard::Key::Multiply, t_actionType);
    }
    else if (str == "/")
    {
        return thor::Action(sf::Keyboard::Key::Divide, t_actionType);
    }
    else if (str == "pause")
    {
        return thor::Action(sf::Keyboard::Key::Pause, t_actionType);
    }
    else if (str == "up")
    {
        return thor::Action(sf::Keyboard::Key::Up, t_actionType);
    }
    else if (str == "down")
    {
        return thor::Action(sf::Keyboard::Key::Down, t_actionType);
    }
    else if (str == "left")
    {
        return thor::Action(sf::Keyboard::Key::Left, t_actionType);
    }
    else if (str == "right")
    {
        return thor::Action(sf::Keyboard::Key::Right, t_actionType);
    }
    else if (str == "f1")
    {
        return thor::Action(sf::Keyboard::Key::F1, t_actionType);
    }
    else if (str == "f2")
    {
        return thor::Action(sf::Keyboard::Key::F2, t_actionType);
    }
    else if (str == "f3")
    {
        return thor::Action(sf::Keyboard::Key::F3, t_actionType);
    }
    else if (str == "f4")
    {
        return thor::Action(sf::Keyboard::Key::F4, t_actionType);
    }
    else if (str == "f5")
    {
        return thor::Action(sf::Keyboard::Key::F5, t_actionType);
    }
    else if (str == "f6")
    {
        return thor::Action(sf::Keyboard::Key::F6, t_actionType);
    }
    else if (str == "f7")
    {
        return thor::Action(sf::Keyboard::Key::F7, t_actionType);
    }
    else if (str == "f8")
    {
        return thor::Action(sf::Keyboard::Key::F8, t_actionType);
    }
    else if (str == "f9")
    {
        return thor::Action(sf::Keyboard::Key::F9, t_actionType);
    }
    else if (str == "f10")
    {
        return thor::Action(sf::Keyboard::Key::F10, t_actionType);
    }
    else if (str == "f11")
    {
        return thor::Action(sf::Keyboard::Key::F11, t_actionType);
    }
    else if (str == "f12")
    {
        return thor::Action(sf::Keyboard::Key::F12, t_actionType);
    }
    else if (str == "mouseleft")
    {
        return thor::Action(sf::Mouse::Left, t_actionType);
    }
    else if (str == "mouseright")
    {
        return thor::Action(sf::Mouse::Right, t_actionType);
    }
    else if (str == "mousemiddle")
    {
        return thor::Action(sf::Mouse::Middle, t_actionType);
    }
    else if (str.substr(0, 3) == "joy" && isInteger(boost::erase_all_copy(str, "joy")))
    {
        return thor::Action(thor::JoystickButton(
                                0,
                                static_cast<unsigned>(std::stoul(boost::erase_all_copy(str, "joy")))),
                            t_actionType);
    }

    throw std::runtime_error("Cannot convert binding '" + t_str + "' to action !");
}

}

} // namespace Rayfun

#endif // UTILITY_HPP
