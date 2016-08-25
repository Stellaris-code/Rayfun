/*  src/texturecontainer.cpp TextureContainer - Yann BOUCHER (yann) 21/08/2016
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
#include "texturecontainer.hpp"

#include <algorithm>

namespace Rayfun
{

TextureContainer::TextureContainer()
{

}

void TextureContainer::addTexture(const std::string &t_name, const sf::Image &t_texture)
{
    m_textures.push_back({t_name, t_texture});
}

sf::Image &TextureContainer::getTexture(const std::string &t_name)
{
    auto iter = std::find_if(m_textures.begin(), m_textures.end(), [t_name](const std::pair<std::string, sf::Image>& t_pair)
    {
        return t_pair.first == t_name;
    });

    if (iter == m_textures.end())
    {
        throw_texturecontainer_error("Cannot find texture '" + t_name + "' in container !");
    }

    return iter->second;
}

const sf::Image &TextureContainer::getTexture(const std::__cxx11::string &t_name) const
{
    auto iter = std::find_if(m_textures.begin(), m_textures.end(), [t_name](const std::pair<std::string, sf::Image>& t_pair)
    {
        return t_pair.first == t_name;
    });

    if (iter == m_textures.end())
    {
        throw_texturecontainer_error("Cannot find texture '" + t_name + "' in container !");
    }

    return iter->second;
}

sf::Image &TextureContainer::getTexture(unsigned char t_id)
{
    if (t_id >= m_textures.size())
    {
        throw_texturecontainer_error("No such id (" + std::to_string(t_id) + ") in texture container !");
    }

    return m_textures[t_id].second;
}

const sf::Image &TextureContainer::getTexture(unsigned char t_id) const
{
    if (t_id >= m_textures.size())
    {
        throw_texturecontainer_error("No such id (" + std::to_string(t_id) + ") in texture container !");
    }

    return m_textures[t_id].second;
}

unsigned char TextureContainer::getId(const std::string &t_name) const
{
    for (size_t i { 0 }; i < m_textures.size(); ++i)
    {
        if (m_textures[i].first == t_name)
        {
            return i + 1;
        }
    }

    return 0;
}

std::vector<sf::Image> TextureContainer::toTextureArray() const
{
    std::vector<sf::Image> textures;

    for (const auto& tex : m_textures)
    {
        textures.push_back(tex.second);
    }

    return textures;
}

}
