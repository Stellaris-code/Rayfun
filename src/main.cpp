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

#include "application.hpp"

int main(int argc, char *argv[])
{
    Rayfun::Application app(sf::VideoMode(1000, 720), "Hey", "testPack.zip", sf::ContextSettings(0, 0, 4), 60);
    app.run();

    return 0;
}
