/*  weaponstructure %{Cpp:License:ClassName} - Yann BOUCHER (yann) 26/06/2016
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
#ifndef WEAPONSTRUCTURE_HPP
#define WEAPONSTRUCTURE_HPP

#include <string>
#include <functional>
#include <vector>

#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Rayfun
{

struct WeaponStructure
{
    std::string name;
    std::string ammoType;
    long magazineSize { -1 };
    sf::Texture spriteSheet;
    thor::FrameAnimation fire_anim;
    sf::Time fireAnimDuration;
    thor::FrameAnimation idle_anim;
    sf::Time idleAnimDuration;
    thor::FrameAnimation reload_anim;
    sf::Time reloadAnimDuration;
    std::function<void()> fire_callback;
    std::function<void()> reload_callback;
};

}

#endif // WEAPONSTRUCTURE_HPP
