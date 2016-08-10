/*  include/pakcontents.hpp PakContents - Yann BOUCHER (yann) 15/04/2016
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
#ifndef PAKCONTENTS_HPP
#define PAKCONTENTS_HPP

#include <string>
#include <vector>

#include <SFML/Audio/Music.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <Thor/Animations/FrameAnimation.hpp>

#include "level.hpp"
#include "weaponstructure.hpp"

namespace Rayfun
{

struct PakContents
{
    std::string title;
    std::string desc;
    std::string author;
    std::string date;
    sf::Texture menuBkg;
    sf::Music menuMusic;
    std::vector<Level> levels;
    std::vector<WeaponStructure> weapons;
};

} // namespace Rayfun

#endif // PAKCONTENTS_HPP
