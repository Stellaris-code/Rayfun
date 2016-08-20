/*  include/shaderrendersurface.hpp ShaderRenderSurface - Yann BOUCHER (yann) 17/08/2016
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
#ifndef SHADERRENDERSURFACE_HPP
#define SHADERRENDERSURFACE_HPP

#include <stdexcept>

#include <SFML/System/Time.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Rayfun
{

class shader_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_shader_error(const std::string& t_why)
{
    throw shader_error("Shader error : " + t_why);
}

class ShaderRenderSurface : public sf::Drawable
{
    public:
        struct SharedInfo
        {
            sf::Vector2f pos { 0.5, 0.5 };
            sf::Vector2f dir { 0, 1 };
        };

    public:
        ShaderRenderSurface(const sf::Vector2f& t_size);

    public:
        void update(const sf::Time&);
        void setSize(const sf::Vector2f& t_size);
        void setTexture(const sf::Texture& t_tex);
        void loadTextureArray(const std::vector<sf::Image>& t_textures);
        void setMap(const sf::Texture& t_map);

    public:
        SharedInfo info;

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    private:
        void updateUniforms();

    private:
        sf::Sprite m_drawSurface;
        sf::Texture m_drawTexture;
        sf::Texture m_textureArray;
        sf::Shader m_renderShader;
};

}

#endif // SHADERRENDERSURFACE_HPP
