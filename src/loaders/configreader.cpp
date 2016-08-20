/*  src/configreader.cpp ConfigReader - Yann BOUCHER (yann) 09/05/2016
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
#include "loaders/configreader.hpp"

#include <json/value.h>

#include "parameters.hpp"

#include "utils/utility.hpp"

namespace Rayfun
{

Parameters Config::parseIntoParameters(const std::string& t_path)
{
    auto get = [t_path](const Json::Value& t_root, const std::string &t_key, bool t_nullIfInvalid = false)
    {
        if (!t_root.isMember(t_key))
        {
            if (t_nullIfInvalid)
            {
                return Json::Value{}; // return null
            }
            else
            {
                throw_configreader_error("cannot find '" + t_key + "' in file " + t_path + " !");
            }
        }
        return t_root[t_key];
    };
    Json::Value root = Utility::fileToJsonTree(t_path);
    Parameters params;
    params.fov = get(root, "fov").asUInt();
    params.bilinear_filtering = get(root, "bilinear_filtering").asBool();
    params.bilinear_sprites = get(root, "bilinear_sprites").asBool();

    return params;
}

} // namespace Rayfun
