/*  imgui-wrapper %{Cpp:License:ClassName} - Yann BOUCHER (yann) 10/08/2016
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
#ifndef IMGUIWRAPPER_HPP
#define IMGUIWRAPPER_HPP

#include <imgui/imgui-SFML.h>

namespace Rayfun
{

class IMGUIWrapper
{
    public:
        IMGUIWrapper(sf::RenderWindow& t_window)
        {
            ImGui::SFML::Init(t_window);
        }

        ~IMGUIWrapper()
        {
            ImGui::SFML::Shutdown();
        }
};

}

#endif // IMGUIWRAPPER_HPP
