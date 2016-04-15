/*  application_error %{Cpp:License:ClassName} - Yann BOUCHER (yann) 15/04/2016
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
#ifndef APPLICATION_ERROR_HPP
#define APPLICATION_ERROR_HPP

#include <stdexcept>

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

} // namespace Rayfun

#endif // APPLICATION_ERROR_HPP
