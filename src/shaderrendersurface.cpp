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

#include <cstring> // for memcpy

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Context.hpp>

#include "utils/graphicsutility.hpp"
#include "glutils.hpp"

#include "constants.hpp"

namespace Rayfun
{

ShaderRenderSurface::ShaderRenderSurface(const sf::Vector2f &t_size, const sf::Vector2f& t_mapSize)
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

    glCheck(glGenBuffers(1, &m_tbo));
    glBindBuffer(GL_TEXTURE_BUFFER, m_tbo);
    glBufferData(GL_TEXTURE_BUFFER, t_mapSize.x * t_mapSize.y * sizeof(uint8_t) * 4, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0 + 30);
    glCheck(glGenTextures(1, &m_map_tex));
    glCheck(glBindTexture(GL_TEXTURE_BUFFER, m_map_tex));
    glCheck(glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8UI, m_tbo));
    glCheck(glBindTexture(GL_TEXTURE_BUFFER, 0));

    sf::Shader::bind(&m_renderShader);
}

ShaderRenderSurface::~ShaderRenderSurface()
{
    glCheck(glDeleteTextures(1, &m_texArray));
    glCheck(glDeleteBuffers(1, &m_tbo));
    glCheck(glDeleteTextures(1, &m_map_tex));
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
    Utility::createTextureArray(m_texArray, t_textures);

    glCheck(glUniform1i(glGetUniformLocation(m_renderShader.getNativeHandle(), "u_textures"), 31));
}

void ShaderRenderSurface::setMap(const std::vector<uint8_t> &t_map, const sf::Vector2s& t_size)
{
    GLuint u_map_loc = glGetUniformLocation(m_renderShader.getNativeHandle(), "u_map");

    m_renderShader.setUniform("u_mapSize", sf::Vector2i(t_size));

    glActiveTexture(GL_TEXTURE0 + 30);

    glBindBuffer(GL_TEXTURE_BUFFER, m_tbo);
    glBufferSubData(GL_TEXTURE_BUFFER, 0, t_map.size(), t_map.data());


    std::vector<uint8_t> data;
    data.resize(t_map.size());

    glGetBufferSubData(GL_TEXTURE_BUFFER, 0, data.size(), data.data());

    glUniform1i(u_map_loc, 30);
}

void ShaderRenderSurface::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    glActiveTexture(GL_TEXTURE0 + 31);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texArray);

    glActiveTexture(GL_TEXTURE0 + 30);
    glBindTexture(GL_TEXTURE_BUFFER, m_map_tex);

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
