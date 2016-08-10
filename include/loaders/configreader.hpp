/*  include/configreader.hpp ConfigReader - Yann BOUCHER (yann) 09/05/2016
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
#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include <stdexcept>
#include <string>

namespace Rayfun
{

struct Parameters;

class configreader_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_configreader_error(const std::string& t_why)
{
    throw configreader_error("Error during config file parsing : " + t_why);
}

namespace Config
{
Parameters parseIntoParameters(const std::string& t_path);
}

} // namespace Rayfun

#endif // CONFIGREADER_HPP
