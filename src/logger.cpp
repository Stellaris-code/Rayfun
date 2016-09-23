/*  src/logger.cpp Logger - Yann BOUCHER (yann) 30/08/2016
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
#include "logger.hpp"

#include <iostream>

#include "hud/console.hpp"

namespace Rayfun
{

void Logger::log(const std::string &t_str, Logger::Level t_logLevel)
{
    if (t_logLevel >= m_minLogLevel)
    {
        if (t_logLevel <= Log)
        {
            if (m_logOstream)
            {
                *m_logOstream << t_str << std::endl;
            }
        }
        else
        {
            if (m_errOstream)
            {
                *m_errOstream << t_str << std::endl;
            }
        }

        if (m_console)
        {
            m_console->log(t_str, t_logLevel);
        }
    }
}

void Logger::setErrOstream(std::ostream *t_ostream)
{
    m_errOstream = t_ostream;
}

void Logger::setLogOstream(std::ostream *t_ostream)
{
    m_logOstream = t_ostream;
}

void Logger::setConsole(Console *t_console)
{
    m_console = t_console;
}

void Logger::setMinLogLevel(Logger::Level t_level)
{
    m_minLogLevel = t_level;
}

}
