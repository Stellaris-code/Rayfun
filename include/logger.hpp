/*  include/logger.hpp Logger - Yann BOUCHER (yann) 30/08/2016
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
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iosfwd>

namespace Rayfun
{

class Console;

class Logger
{
    public:
        enum Level
        {
            Log,
            Warn,
            Error,
            Crit
        };

    public:
        void log(const std::string& t_str, Level t_logLevel = Level::Log);

        void setErrOstream(std::ostream* t_ostream);
        void setLogOstream(std::ostream* t_ostream);
        void setConsole(Console* t_console);
        void setMinLogLevel(Level t_level);

        Console* console() { return m_console; }
        const Console* console() const { return m_console; }

    private:
        std::ostream* m_errOstream { nullptr };
        std::ostream* m_logOstream { nullptr };
        Console* m_console { nullptr };
        Level m_minLogLevel { Level::Log };
};

}

#endif // LOGGER_HPP
