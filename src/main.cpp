/*  %{Cpp:License:FileName} %{Cpp:License:ClassName} - Yann BOUCHER (yann) 10/04/2016
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
#include <iostream>

#include <Thor/Vectors.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "raycasting.hpp"
#include "map.hpp"
#include "tile.hpp"
#include "camera.hpp"
#include "sector.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    sf::Image image;
    image.loadFromFile("data/greystone.png");

    sf::Image image2;
    image2.loadFromFile("data/mossy.png");

    Rayfun::Tile wall;
    wall.isWall = true;
    wall.clip.set(true);
    wall.tag = "DD";
    wall.tex.set(image);

    Rayfun::Tile wall2;
    wall2.isWall = true;
    wall2.clip.set(true);
    wall2.tag = "AA";
    wall2.tex.set(image2);

    Rayfun::Map map({ 4, 4 });
    map.setTileAt({ 0, 0}, wall2);
    map.setTileAt({ 1, 0}, wall2);
    map.setTileAt({ 2, 0}, wall2);
    map.setTileAt({ 3, 0}, wall2);

    map.setTileAt({ 0, 1}, wall);
    map.setTileAt({ 0, 2}, wall);
    map.setTileAt({ 3, 1}, wall);
    map.setTileAt({ 3, 2}, wall);

    map.setTileAt({ 0, 3}, wall);
    map.setTileAt({ 1, 3}, wall);
    map.setTileAt({ 2, 3}, wall);
    map.setTileAt({ 3, 3}, wall);

    Rayfun::Sector sector { {0, 0, 4, 4 }, image, image2 };
    map.sectors.emplace_back(sector);

    auto result = Rayfun::Raycasting::castRay({ 1,1 }, { -0.7, 0.7 }, map);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Default,
                            sf::ContextSettings(0, 0, 4));

    Rayfun::Camera camera(sf::Vector2s(window.getSize() / 4U), { 2, 2 }, 90, 90);

    sf::Texture texture;
    texture.create(sf::Vector2u(camera.screenSize()).x, sf::Vector2u(camera.screenSize()).y);

    sf::Sprite sprite(texture);
    sprite.setPosition(0, 0);
    sprite.setScale(4, 4);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            camera.setAngle(camera.angle() - 3);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            camera.setAngle(camera.angle() + 3);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            camera.setPos(camera.pos() + thor::rotatedVector(sf::Vector2d(0.1, 0),
                                                             thor::polarAngle(camera.direction())));
            //std::cerr << camera.pos().x << " : " << camera.pos().y << "\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            camera.setPos(camera.pos() - thor::rotatedVector(sf::Vector2d(0.1, 0),
                                                             thor::polarAngle(camera.direction())));
        }

        texture.update(Rayfun::Raycasting::render(camera, map).data());

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
