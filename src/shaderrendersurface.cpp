/*  src/shaderrendersurface.cpp ShaderRenderSurface - Yann BOUCHER (yann) 17/08/2016
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
#include "shaderrendersurface.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utils/graphicsutility.hpp"

#include "constants.hpp"

namespace Rayfun
{

ShaderRenderSurface::ShaderRenderSurface(const sf::Vector2f &t_size)
{
    if (!sf::Shader::isAvailable())
    {
        throw_shader_error("Shaders are not supported on this machine !");
    }

    m_drawTexture.create(t_size.x, t_size.y);

    m_renderShader.loadFromFile(c_glRenderShader, sf::Shader::Fragment);

    m_drawSurface.setTexture(m_drawTexture);
}

void ShaderRenderSurface::update(const sf::Time &)
{
    updateUniforms();
}

void ShaderRenderSurface::setSize(const sf::Vector2f &t_size)
{
    m_drawTexture.create(t_size.x, t_size.y);
}

void ShaderRenderSurface::loadTextureArray(const std::vector<sf::Image> &t_textures)
{
    Utility::createTextureArray(m_textureArray, t_textures);

    m_renderShader.setUniform("u_textures", m_textureArray);
}

void ShaderRenderSurface::setMap(const sf::Texture &t_map)
{
    m_renderShader.setUniform("u_map", t_map);
}

void ShaderRenderSurface::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.shader = &m_renderShader;
    t_target.draw(m_drawSurface, t_states);
}

void ShaderRenderSurface::updateUniforms()
{
    m_renderShader.setUniform("u_resolution", sf::Vector2f(m_drawTexture.getSize()));
    m_renderShader.setUniform("u_pos", info.pos);
    m_renderShader.setUniform("u_dir", info.dir);
}

}
