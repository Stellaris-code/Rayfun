/*  include/gamestate.hpp GameState - Yann BOUCHER (yann) 07/05/2016
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
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Thor/Input/EventSystem.hpp>
#include <Thor/Input/ActionMap.hpp>

#include "player.hpp"

#include "state.hpp"

#include "billboard.hpp"
#include "animatedbillboard.hpp"
#include "weapon.hpp"
#include "level.hpp"
#include "pakcontents.hpp"

#include "hud/minimap.hpp"
#include "hud/hud.hpp"

namespace Rayfun
{

class GameState : public State
{
    public:
        explicit GameState(State::Context& t_context);

    public:
        virtual void pause() override;
        virtual void resume() override;


        virtual void handleEvent(const sf::Event&) override;

        virtual void update(const sf::Time& t_elapsedTime) override;

        virtual void display() override;

        virtual void initChai() override;

    public:
        Level& currentLevel() { return m_context.mapPack.levels[m_currentLevel]; }
        const Level& currentLevel() const { return m_context.mapPack.levels[m_currentLevel]; }

    private:
        Player m_player;
        Camera m_cam;
        sf::Time m_lastDeltaTime { sf::Time::Zero };
        sf::Sprite m_screenSprite;
        sf::Texture m_renderTexture;
        thor::ActionMap<std::string>::CallbackSystem m_callbacks;
        Minimap m_minimap;
        size_t m_currentLevel { 0 };

        // debug

        Billboard bill;
        AnimatedBillboard bill2;
        HUD hud;
};

} // namespace Rayfun

#endif // GAMESTATE_HPP
