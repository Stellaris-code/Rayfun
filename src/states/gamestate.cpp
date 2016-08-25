/*  src/gamestate.cpp GameState - Yann BOUCHER (yann) 07/05/2016
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

#include "states/gamestate.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <Thor/Vectors.hpp>
#include <random>

#include "parameters.hpp"
#include "pakcontents.hpp"
#include "player.hpp"

#include "raycasting.hpp"
#include "utils/graphicsutility.hpp"
#include "utils/mathutility.hpp"
#include "utils/gameplayglobals.hpp"
#include "utils/glslinterop.hpp"

#include "statemachine.hpp"

#include <iostream>
#include <Thor/Math.hpp>

namespace Rayfun
{

GameState::GameState(Context &t_context)
    : State(t_context), m_cam(sf::Vector2s(m_context.window.getSize()) / 1UL, { 0, 0 },
                              t_context.params.fov, 0), m_screenSprite(m_renderTexture),
      m_player(m_context),
      m_minimap(m_context.mapPack.level.map, m_context.resources, sf::Vector2f(m_context.window.getSize())),
      m_renderSurface(sf::Vector2f(m_context.window.getSize())),
      bill(m_context), bill2(m_context),
      hud(m_context.resources)
{
    m_renderTexture.create(sf::Vector2u(m_cam.screenSize()).x, sf::Vector2u(m_cam.screenSize()).y);
    bill2.setSpriteSheet(m_context.resources.imageHolder["spritesheets/doom_items"]);
    m_screenSprite.setTexture(m_renderTexture, true);
    m_screenSprite.setPosition(0, 0);
    m_screenSprite.setScale(1, 1);

    m_context.resources.textureHolder["textures/stone"].setSmooth(true);

    m_renderSurface.loadTextureArray(m_context.mapPack.textures.toTextureArray());

    bill2.addFrame(sf::seconds(0.1f), sf::IntRect(1, 83, 26, 96));
    bill2.addFrame(sf::seconds(0.1f), sf::IntRect(31, 83, 26, 96));
    bill2.addFrame(sf::seconds(0.1f), sf::IntRect(61, 83, 26, 96));
    bill2.addFrame(sf::seconds(0.1f), sf::IntRect(91, 84, 26, 96));

    sf::Image img;
    img.create(41, 56, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {0, 0, 41, 56});
    bill.setImage(img, 0, 45 - 22.5);
    bill.setImage(img, 360 - 22.5, 360);
    img.create(35, 53, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {52, 0, 35, 53});
    img.flipHorizontally();
    bill.setImage(img, 45 - 22.5, 90 - 22.5);
    img.create(43, 53, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {101, 0, 43, 53});
    img.flipHorizontally();
    bill.setImage(img, 90 - 22.5, 135 - 22.5);
    img.create(45, 51, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {162, 0, 45, 51});
    img.flipHorizontally();
    bill.setImage(img, 135 - 22.5, 180 - 22.5);
    img.create(36, 50, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {216, 0, 36, 50});
    bill.setImage(img, 180 - 22.5, 225 - 22.5);

    img.create(45, 51, sf::Color::White);
    img.copy(m_context.resources.imageHolder["marine"], 0, 0, {162, 0, 45, 51});
    bill.setImage(img, 225 - 22.5, 270 - 22.5);


    bill.scale = {0.75, 0.75};

    hud.setSize(sf::Vector2d(m_cam.screenSize()));
    hud.setPosition(0, m_cam.screenSize().y);


    m_player.ammo("bullets") = 10;
    m_player.addWeapon(m_context.mapPack.weapons[0]);
    m_player.weapons[0].setScale(2*1.5, 2*1.5);
    m_player.weapons[0].setSpriteSheet(m_context.resources.textureHolder["spritesheets/doom_weapons"]);;
    hud.setWeapon(&m_player.weapons[0]);

    initChai();

    m_callbacks.connect0("turnright", [this](){
        m_player.turn(135 * m_lastDeltaTime.asSeconds());
    });
    m_callbacks.connect0("turnleft", [this](){
        m_player.turn(-135 * m_lastDeltaTime.asSeconds());
    });
    m_callbacks.connect0("straferight", [this](){
        m_player.strafe(4 * m_lastDeltaTime.asSeconds());
    });
    m_callbacks.connect0("strafeleft", [this](){
        m_player.strafe(-4 * m_lastDeltaTime.asSeconds());
    });
    m_callbacks.connect0("moveforward", [this](){
        m_player.move(4 * m_lastDeltaTime.asSeconds());
    });
    m_callbacks.connect0("movebackward", [this](){
        m_player.move(-4 * m_lastDeltaTime.asSeconds());
    });

    m_callbacks.connect0("fire", [this]()
    {
        m_player.firePrimary();
    });
    m_callbacks.connect0("use", [this]()
    {
        m_player.use();
    });

    bill2.pos = { 2.5, 2.5 };
    bill2.z = 26;
    bill.pos = { 3.5, 3.5 };
    m_player.pos = { 1.5, 1.5 };
}

void GameState::pause()
{

}

void GameState::resume()
{

}

void GameState::handleEvent(const sf::Event& t_event)
{
    if (t_event.type == sf::Event::Resized)
    {
        m_renderSurface.setSize(sf::Vector2f(m_context.window.getSize()));
    }
}

void GameState::update(const sf::Time& t_deltaTime)
{
    m_lastDeltaTime = t_deltaTime;
    m_context.params.bindings.invokeCallbacks(m_callbacks, &m_context.window);

    for (auto& t : m_context.mapPack.level.map.triggers)
    {
        if (t.rect.contains(sf::Vector2s(m_player.pos)))
        {
            if (!t.once || !t.detail_alreadytriggered)
            {
                if (!t.detail_entered)
                {
                    t.detail_entered = true;
                    t.detail_alreadytriggered = true;
                    Actor hey(m_context);
                    t.ontrigger(hey);
                }
            }
        }
        else
        {
            t.detail_entered = false;
        }
    }

    bill.playerPos = m_player.pos;
    bill.update(t_deltaTime);
    bill2.update(t_deltaTime);
    m_context.mapPack.level.map.playerPos = m_player.pos;
    m_context.mapPack.level.map.update(t_deltaTime);

    m_player.update(t_deltaTime);

    hud.setPlayerInfo({ m_player.health, m_player.armor, m_player.ammo_map()});
    //hud.moveSpeed = m_player.movementAmount();
    hud.update(t_deltaTime);

    m_cam.setPos(m_player.pos);
    m_cam.setAngle(m_player.angle);

    m_minimap.playerPos = sf::Vector2f(m_player.pos);
    m_minimap.playerAngle = static_cast<float>(m_player.angle + 90);
    m_minimap.update(t_deltaTime);

    if (m_context.params.bindings.isActive("strafeleft"))
    {
        m_context.stateMachine.popState();
    }

    m_renderSurface.info.pos = sf::Vector2f(m_cam.pos());
    m_renderSurface.info.dir = sf::Vector2f(m_cam.direction());
    m_renderSurface.setMap(Utility::mapToBuffer(m_context.mapPack.level.map), m_context.mapPack.level.map.size());
    m_renderSurface.update(t_deltaTime);
        m_screenSprite.setTexture(m_renderTexture);
}

void GameState::display()
{
    // m_renderTexture.update(Raycasting::render(m_cam, m_context.mapPack.level.map,
     //                       m_context.params.bilinear_filtering, m_context.params.bilinear_sprites).data());
    //m_renderTexture =  Utility::mapToTexture(m_context.mapPack.level.map);
    // m_context.window.draw(m_screenSprite);
    m_context.window.draw(m_renderSurface);
    m_context.window.draw(hud);
    if (m_context.params.bindings.isActive("minimap"))
    {
        m_context.window.draw(m_minimap);
    }
}

} // namespace Rayfun
