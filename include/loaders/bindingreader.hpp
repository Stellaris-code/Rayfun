/*  include/bindingreader.hpp BindingReader - Yann BOUCHER (yann) 08/05/2016
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
#ifndef BINDINGREADER_HPP
#define BINDINGREADER_HPP

#include <Thor/Input/ActionMap.hpp>

#include <string>
#include <map>

namespace Json
{
class Value;
}

namespace Rayfun
{
class bindingreader_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_bindingreader_error(const std::string& t_why)
{
    throw bindingreader_error("Error during binding file parsing : " + t_why);
}

namespace Binding
{
thor::ActionMap<std::string> jsonToActionMap(const std::string& t_path,
                                             std::map<std::string, thor::Action::ActionType> t_actionTypeMap);
}

} // namespace Rayfun

#endif // BINDINGREADER_HPP
