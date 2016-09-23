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

#include "cmake/config.hpp"

#ifdef FLTK_FOUND

#include <FL/fl_message.H>

#elif defined(SDL2_FOUND)

#include <SDL2/SDL_messagebox.h>

#endif

int main(int argc, char *argv[])
{
    try
    {
        Rayfun::Application app(sf::VideoMode(1000, 720), "Hey", "testPack.zip", sf::ContextSettings(0, 0, 4), 144);

        app.run();
    }
    catch (const std::exception& e)
    {
#ifdef FLTK_FOUND
        fl_alert("%s", e.what());
#elif defined(SDL2_FOUND)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#else
        std::cerr << e.what() << "\n";
#endif
        return 1;
    }

    return 0;
}
