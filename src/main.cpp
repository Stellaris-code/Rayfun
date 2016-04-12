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

#include "raycasting.hpp"
#include "map.hpp"
#include "tile.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;

    Rayfun::Tile wall;
    wall.isWall = true;
    wall.clip.set(true);
    wall.tag = "DD";
    Rayfun::Map map({ 4, 4 });
    map.setTileAt({ 0, 1}, wall);
    map.setTileAt({ 1, 1}, wall);
    map.setTileAt({ 2, 1}, wall);
    map.setTileAt({ 3, 1}, wall);

    auto result = Rayfun::Raycasting::castRay({ 3,0 }, { -0.7, 0.7 }, map);

    return 0;
}
