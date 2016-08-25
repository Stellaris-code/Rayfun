/*  include/texturecontainer.hpp TextureContainer - Yann BOUCHER (yann) 21/08/2016
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
#ifndef TEXTURECONTAINER_HPP
#define TEXTURECONTAINER_HPP

#include <functional> // std::pair
#include <stdexcept>
#include <string>
#include <vector>

#include <SFML/Graphics/Image.hpp>

namespace Rayfun
{

class texturecontainer_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_texturecontainer_error(const std::string& t_why)
{
    throw texturecontainer_error("Texture container error : " + t_why);
}

class TextureContainer
{
    public:
        TextureContainer();

    public:
        void addTexture(const std::string& t_name, const sf::Image& t_texture);

        sf::Image& getTexture(const std::string& t_name);
        const sf::Image& getTexture(const std::string& t_name) const;

        sf::Image& getTexture(unsigned char t_id);
        const sf::Image& getTexture(unsigned char t_id) const;

        unsigned char getId(const std::string& t_name) const;

        std::vector<sf::Image> toTextureArray() const;

    private:
        std::vector<std::pair<std::string, sf::Image>> m_textures;
};

}

#endif // TEXTURECONTAINER_HPP
