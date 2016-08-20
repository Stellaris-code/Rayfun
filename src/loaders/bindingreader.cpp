/*  src/bindingreader.cpp BindingReader - Yann BOUCHER (yann) 08/05/2016
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
#include "loaders/bindingreader.hpp"

#include <fstream>

#include <json/value.h>
#include <json/reader.h>

#include "utils/utility.hpp"

namespace Rayfun
{

thor::ActionMap<std::string> Binding::jsonToActionMap(const std::string& t_path,
                                                      std::map<std::string, thor::Action::ActionType> t_actionTypeMap)
{
    Json::Value root = Utility::fileToJsonTree(t_path);
    thor::ActionMap<std::string> actionMap;
    for (auto s : root.getMemberNames())
    {
        if (!root[s].isString())
        {
            if (root[s].isArray())
            {
                actionMap[s] = thor::eventAction([](const sf::Event&) { return true; });
                for (auto key : root[s])
                {
                    if (!key.isString())
                    {
                        throw_bindingreader_error("Binding '" + s +"' must be a string !");
                    }
                    else
                    {
                        actionMap[s] = actionMap[s] || Utility::stringToAction(key.asString(),
                                                                               t_actionTypeMap.at(s));
                    }
                }
                continue;
            }
            else
            {
                throw_bindingreader_error("Binding '" + s + "' must be a string or an array of string !");
            }
        }
        if (t_actionTypeMap.find(s) == t_actionTypeMap.end())
        {
            throw_bindingreader_error("Binding '" + s + "' doesn't exist !");
        }
        actionMap[s] = Utility::stringToAction(root[s].asString(), t_actionTypeMap.at(s));
    }

    return actionMap;
}

} // namespace Rayfun

