/*  gamestate_chaidef GameState - Yann BOUCHER (yann) 16/05/2016
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

#include "gamestate.hpp"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>
#include <iostream>

#include "gameplayglobals.hpp"
#include "pakcontents.hpp"

namespace Rayfun
{

void GameState::initChai()
{
    chaiscript::ModulePtr module = std::make_shared<chaiscript::Module>();

    module->add(chaiscript::base_class<Actor, Player>());

    module->add(chaiscript::fun([this]() -> Player& { return m_player; }), "getPlayer");

    module->add(chaiscript::fun([this]() -> Map& { return m_context.mapPack.levels[m_context.currentLevelIndex].map; }), "getMap");

    module->add(chaiscript::fun([this](Actor& t_actor, sf::Vector2d t_src, sf::Vector2d t_dir, sf::Vector2d t_spread,
                                size_t t_dmg, size_t t_num, std::string t_dmgType, size_t t_maxDistance)
    { fireBullets(t_actor, t_src, t_dir, t_spread, m_context.mapPack.levels[m_context.currentLevelIndex].map, m_context, t_dmg,
      t_num, t_dmgType, t_maxDistance); }), "fireBullets");

    m_context.scriptEngine.add(module);
}

} // namespace Rayfun
