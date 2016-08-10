/*  chaiinterfaceloader %{Cpp:License:ClassName} - Yann BOUCHER (yann) 11/05/2016
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

#include "chaiinterfaceloader.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Thor/Resources/ResourceHolder.hpp>
#include <Thor/Math/Trigonometry.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>
#include <chaiscript/extras/math.hpp>

#include "gameplayglobals.hpp"
#include "mathutility.hpp"

#include "actor.hpp"
#include "map.hpp"
#include "tile.hpp"
#include "common.hpp"


namespace Rayfun
{

void loadChaiInterfaces(chaiscript::ChaiScript& t_chai, const Resources& t_res)
{
    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
    chaiscript::extras::math::bootstrap(m);

    t_chai.add(chaiscript::fun([&t_chai](const chaiscript::Boxed_Value &obj, const std::string &name)
    {
        auto state = t_chai.get_state();
        if (state.engine_state.m_global_objects.find(name) == state.engine_state.m_global_objects.end())
        {
            t_chai.add_global(obj, name);
        }

    }), "add_script_global");

    m->add(chaiscript::fun([]{
        std::cerr << "dd\n";}),
           "err");

    m->add(chaiscript::fun([&t_res](const std::string& t_id) { return t_res.imageHolder[t_id]; }), "loadImage");
    m->add(chaiscript::fun([&t_res](const std::string& t_id) { return t_res.textureHolder[t_id]; }), "loadTexture");

    m->add(chaiscript::fun([&t_res](const std::string& t_id, sf::Image& t_img)
    { t_img = t_res.imageHolder[t_id]; }), "loadImage");
    m->add(chaiscript::fun([&t_res](const std::string& t_id, sf::Texture& t_img)
    { t_img = t_res.textureHolder[t_id]; }), "loadTexture");

    m->add(chaiscript::user_type<sf::Image>(), "Image");
    m->add(chaiscript::fun(&sf::Image::operator=), "=");
    m->add(chaiscript::user_type<sf::Texture>(), "Texture");
    m->add(chaiscript::fun(&sf::Texture::operator=), "=");

    addSfVectorClass<int>(m, "i");
    addSfVectorClass<unsigned>(m, "u");
    addSfVectorClass<float>(m, "f");
    addSfVectorClass<double>(m, "d");
    addSfVectorClass<size_t>(m, "s");

    chaiscript::utility::add_class<Tile>(*m, "Tile",
    {chaiscript::constructor<Tile()>()}, {
                                             {chaiscript::fun(&Tile::tex), "tex"},
                                             {chaiscript::fun(&Tile::clip), "clip"},
                                             {chaiscript::fun(&Tile::isWall), "isWall"}
                                         });

    addDirectionalClass<bool>(m, "Bool");
    addDirectionalClass<sf::Image>(m, "Image");
    addDirectionalClass<std::shared_ptr<sf::Image>>(m, "Image");

    m->add_global_const(chaiscript::const_var(Side::North), "North");
    m->add_global_const(chaiscript::const_var(Side::South), "South");
    m->add_global_const(chaiscript::const_var(Side::East), "East");
    m->add_global_const(chaiscript::const_var(Side::West), "West");

    m->add(chaiscript::user_type<Side>(), "Side");

    chaiscript::utility::add_class<Actor>(*m, "Actor",
    {
                                          },
    {
                                              { chaiscript::fun(&Actor::pos), "pos" },
                                              { chaiscript::fun(&Actor::angle), "angle"}
                                          });

    m->add(chaiscript::fun([](Tile& t_tile, Side t_tileSide, Side t_slideDir, float t_duration)
    {
               if (!t_tile.sliding)
               {
                   t_tile.slideSide = t_tileSide;
                   t_tile.slideDirection = t_slideDir;
                   t_tile.slideSpeed = sf::seconds(t_duration);
                   t_tile.slideProgress = sf::Time::Zero;
                   t_tile.sliding = true;
               }
           }), "slideTile");

    m->add(chaiscript::fun([&t_res](const std::string& t_sound, sf::Vector2f t_pos, float t_volume = 100) -> void
    {
        playSoundAt(t_res, t_sound, t_pos, t_volume);
    }), "playSoundAt");

    m->add(chaiscript::fun([&t_res](const std::string& t_sound, float t_volume = 100) -> void
    {
        playSoundUI(t_res, t_sound, t_volume);
    }), "playSoundUI");

    m->add(chaiscript::fun([](const sf::Vector2d& t_dir) { return thor::polarAngle(t_dir);}), "dirToAngle");
    m->add(chaiscript::fun([](double t_angle) { return Utility::angleToVector(t_angle);}), "angleToDir");

    m->add(chaiscript::fun([](double t_rad) { return thor::toDegree(t_rad);}), "toDegree");
    m->add(chaiscript::fun([](double t_deg) { return thor::toRadian(t_deg);}), "toRadian");

    m->add(chaiscript::user_type<Map>(), "Map");
    m->add(chaiscript::fun<Tile&, Map, const sf::Vector2s&>(&Map::tileAt), "tileAt");
    m->add(chaiscript::fun<const Tile&, Map, const sf::Vector2s&>(&Map::tileAt), "tileAt");


    t_chai.add(m);

}
}
