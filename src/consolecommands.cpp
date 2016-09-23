/*  consolecommands %{Cpp:License:ClassName} - Yann BOUCHER (yann) 19/09/2016
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

#include <fstream>

#include <boost/lexical_cast.hpp>

#include <chaiscript/chaiscript.hpp>

namespace Rayfun
{

void Application::initCommands()
{
    m_console.setCvar("mouselook", [this]{ return std::to_string(m_params.mouselook);},
    [this](const std::string& val)
    {
        if (val == "0" || val == "false")
        {
            m_params.mouselook = false;
        }
        else if (val == "1" || val == "true")
        {
            m_params.mouselook = true;
        }
        else
        {
            m_logger.log("Cannot convert " + val + " to bool !", Logger::Error);
        }
    });

    m_console.setCvar("bilinear_sprites", [this]{ return std::to_string(m_params.bilinear_sprites);},
    [this](const std::string& val)
    {
        if (val == "0" || val == "false")
        {
            m_params.bilinear_sprites = false;
        }
        else if (val == "1" || val == "true")
        {
            m_params.bilinear_sprites = true;
        }
        else
        {
            m_logger.log("Cannot convert " + val + " to bool !", Logger::Error);
        }
    });

    m_console.setCvar("bilinear_filtering", [this]{ return std::to_string(m_params.bilinear_filtering);},
    [this](const std::string& val)
    {
        if (val == "0" || val == "false")
        {
            m_params.bilinear_filtering = false;
        }
        else if (val == "1" || val == "true")
        {
            m_params.bilinear_filtering= true;
        }
        else
        {
            m_logger.log("Cannot convert " + val + " to bool !", Logger::Error);
        }
    });

    m_console.setCvar("mouse_sensitivity", [this]{ return std::to_string(m_params.mouseSensitivity);},
    [this](const std::string& val)
    {
        try
        {
            m_params.mouseSensitivity = boost::lexical_cast<double>(val);
        }
        catch (const boost::bad_lexical_cast& e)
        {
            m_logger.log("Cannot convert " + val + " to double !", Logger::Error);
        }
    });

    m_console.setCallback("quit", [this](const std::vector<std::string>&)
    {
        m_logger.log("Exiting application");
        m_window.close();
    });

    m_console.setCallback("echo", [this](const std::vector<std::string>& t_args)
    {
        std::string str;
        for (const auto& s : t_args)
        {
            str += s + " ";
        }
        m_logger.log(str);
    });

    m_console.setCallback("exec", [this](const std::vector<std::string>& t_args)
    {
        if (t_args.size() != 1)
        {
            m_logger.log("exec must be used with a filename as argument ! ", Logger::Error);
            return;
        }
        std::ifstream file(t_args[0]);
        if (!file)
        {
            m_logger.log("Cannot open '" + t_args[0] + "' !", Logger::Error);
        }
        else
        {
            std::string line;
            while (std::getline(file, line))
            {
                m_console.sendCommand(line);
            }
        }
    });

    m_console.setCallback("clear", [this](const std::vector<std::string>&)
    {
        m_console.clear();
    });

    m_console.setCallback("chai", [this](const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            m_logger.log("Only one argument is accepted for chai command !", Logger::Error);
            return;
        }
        try
        {
            m_scriptEngine.eval(args[0]);
        }
        catch (const chaiscript::exception::eval_error& e)
        {
            m_logger.log(e.pretty_print(), Logger::Error);
        }

    });

    m_console.setCallback("chai_file", [this](const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            m_logger.log("Only one argument is accepted for chai_file command !", Logger::Error);
            return;
        }
        try
        {
            m_scriptEngine.eval_file(args[0]);
        }
        catch (const chaiscript::exception::file_not_found_error& e)
        {
            m_logger.log(e.what(), Logger::Error);
            return;
        }
        catch (const chaiscript::exception::eval_error& e)
        {
            m_logger.log(e.pretty_print(), Logger::Error);
            return;
        }
    });
}

}
