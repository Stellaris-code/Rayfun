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
#include "glutils.hpp"

#include "constants.hpp"

#include <iostream>

namespace Rayfun
{

ShaderRenderSurface::ShaderRenderSurface(const sf::Vector2f &t_size)
{
    if (!sf::Shader::isAvailable())
    {
        throw_shader_error("Shaders are not supported on this machine !");
    }

    m_drawTexture.create(t_size.x, t_size.y);

    if (!m_renderShader.loadFromFile(c_glRenderShader, sf::Shader::Fragment))
    {
        throw_shader_error("Error loading shader '" + std::string(c_glRenderShader) + "' !");
    }

    m_drawSurface.setTexture(m_drawTexture);

    glCheck(glGenTextures(1, &m_texArray));

    sf::Shader::bind(&m_renderShader);

    glCheck(glUniform1i(glGetUniformLocation(m_renderShader.getNativeHandle(), "u_textures"), 31));

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
    sf::Shader::bind(&m_renderShader);

    Utility::createTextureArray(m_texArray, t_textures);

    glCheck(glActiveTexture(GL_TEXTURE0));
}

void ShaderRenderSurface::setMap(const std::vector<uint8_t> &t_map, const sf::Vector2s& t_size)
{
    glCheck(glGenBuffers(1, &m_tbo));

    glCheck(glBindBuffer(GL_TEXTURE_BUFFER, m_tbo));

    glCheck(glBufferData(GL_TEXTURE_BUFFER, t_map.size() * sizeof(uint8_t), t_map.data(), GL_STATIC_DRAW));


    m_mapSize = t_size;

    glCheck(glGenTextures(1, &m_map_tex));

    glCheck(glActiveTexture(GL_TEXTURE2));

    glCheck(glBindTexture(GL_TEXTURE_BUFFER, m_map_tex));

    glCheck(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8UI, m_tbo));

    GLuint u_map_loc = glGetUniformLocation(m_renderShader.getNativeHandle(), "u_map");

    glUseProgram(m_renderShader.getNativeHandle());

    glCheck(glUniform1i(u_map_loc, 2));
    m_renderShader.setUniform("u_mapSize", sf::Vector2i(t_size));


    glCheck(glDeleteBuffers(1, &m_tbo));
    glCheck(glDeleteTextures(1, &m_map_tex));
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
