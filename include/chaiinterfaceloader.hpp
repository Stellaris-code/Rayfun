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
#ifndef CHAIINTERFACELOADER_HPP
#define CHAIINTERFACELOADER_HPP

#include <SFML/System/Vector2.hpp>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>

#include "common.hpp"

namespace Rayfun
{

template <typename T>
inline void addSfVectorClass(chaiscript::ModulePtr t_module, const std::string& t_suffix)
{
    chaiscript::utility::add_class<sf::Vector2<T>>(*t_module, "Vector2" + t_suffix,
    {
                                                       chaiscript::constructor<sf::Vector2<T>()>(),
                                                       chaiscript::constructor<sf::Vector2<T>(T, T)>(),
                                                       chaiscript::constructor<sf::Vector2<T>(const sf::Vector2<T>&)>()},
    {
                                                       {chaiscript::fun(&sf::Vector2<T>::x), "x"},
                                                       {chaiscript::fun(&sf::Vector2<T>::y), "y"}
                                                   }
                                                   );
    t_module->add(chaiscript::fun([](const sf::Vector2<T>& t_vec) -> sf::Vector2<T>
    { return -t_vec; }), "-");
    t_module->add(chaiscript::fun([](sf::Vector2<T>& t_left, const sf::Vector2<T>& t_right) -> sf::Vector2<T>&
    { return t_left += t_right; }), "+=");
    t_module->add(chaiscript::fun([](sf::Vector2<T>& t_left, const sf::Vector2<T>& t_right) -> sf::Vector2<T>&
    { return t_left -= t_right; }), "-=");
    t_module->add(chaiscript::fun([](const sf::Vector2<T>& t_left, const sf::Vector2<T>& t_right) -> sf::Vector2<T>
    { return t_left + t_right; }), "+");
    t_module->add(chaiscript::fun([](const sf::Vector2<T>& t_left, const sf::Vector2<T>& t_right) -> sf::Vector2<T>
    { return t_left - t_right; }), "-");
    t_module->add(chaiscript::fun([](const sf::Vector2<T>& t_left, T t_right) -> sf::Vector2<T>
    { return t_left * t_right; }), "*");
    t_module->add(chaiscript::fun([](T t_left, const sf::Vector2<T>& t_right) -> sf::Vector2<T>
    { return t_left * t_right; }), "*");
    t_module->add(chaiscript::fun([](const sf::Vector2<T>& t_left, T t_right) -> sf::Vector2<T>
    { return t_left / t_right; }), "/");
    t_module->add(chaiscript::fun([](sf::Vector2<T>& t_left, T t_right) -> sf::Vector2<T>&
    { return t_left *= t_right; }), "*=");
    t_module->add(chaiscript::fun([](sf::Vector2<T>& t_left, T t_right) -> sf::Vector2<T>&
    { return t_left /= t_right; }), "/=");
}

template <typename T>
inline void addDirectionalClass(chaiscript::ModulePtr t_module, const std::string& t_suffix)
{
    chaiscript::utility::add_class<Directional<T>>(*t_module, "Directional" + t_suffix,
    {
      chaiscript::constructor<Directional<T>()>(),
      chaiscript::constructor<Directional<T>(T)>()
    },
    {
      {chaiscript::fun(&Directional<T>::set), "set"},
      {chaiscript::fun<T&, Directional<T>, Side>(&Directional<T>::operator[]), "[]"},
      {chaiscript::fun<const T&, Directional<T>, Side>(&Directional<T>::operator[]), "[]"}
    });
}

void loadChaiInterfaces(chaiscript::ChaiScript& t_chai, const Resources &t_res);

}

#endif // CHAIINTERFACELOADER_HPP
