/*  graphicsutility %{Cpp:License:ClassName} - Yann BOUCHER (yann) 12/05/2016
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

#include <GL/glew.h>

#include "utils/graphicsutility.hpp"

#include <cassert>

#include <stdexcept>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <SFML/OpenGL.hpp>

#include <SFML/System/Vector2.hpp>

#include <Thor/Math/Random.hpp>
#include <Thor/Graphics/ColorGradient.hpp>

#include <iostream>

#include "utils/mathutility.hpp"
#include "glutils.hpp"

#include "common.hpp"

namespace Rayfun
{
namespace Utility
{

void addDecal(sf::Image& t_target, sf::Image t_decal, const sf::Vector2s &t_pos,
              bool t_randomFlip)
{
    assert(t_pos.x <= t_target.getSize().x && t_pos.y <= t_target.getSize().y);

    if (t_randomFlip)
    {
        if (thor::random(0, 1))
        {
            t_decal.flipHorizontally();
        }
        if (thor::random(0, 1))
        {
            t_decal.flipVertically();
        }
    }

    for (size_t x { 0 }; x < t_decal.getSize().x; ++x)
    {
        for (size_t y { 0 }; y < t_decal.getSize().y; ++y)
        {
            int pos_x = x + t_pos.x;
            int pos_y = y + t_pos.y;
            if (pos_x >= 0 && pos_x < t_target.getSize().x &&
                    pos_y >= 0 && pos_y < t_target.getSize().y)
            {
                sf::Color color = thor::blendColors(t_target.getPixel(pos_x, pos_y),
                                                    t_decal.getPixel(x, y), t_decal.getPixel(x, y).a / 255.f);
                t_target.setPixel(pos_x, pos_y, color);
            }
        }
    }
}

bool imageContainsAlpha(const sf::Image& t_image)
{
    for (unsigned x { 0 }; x < t_image.getSize().x; ++x)
    {
        for (unsigned y { 0 }; y < t_image.getSize().y; ++y)
        {
            if (t_image.getPixel(x, y).a != 0xFF)
            {
                return true;
            }
        }
    }
    return false;
}

bool imageStripContainsAlpha(const sf::Image& t_image, unsigned t_column)
{
    assert(t_column <= t_image.getSize().x);
    for (unsigned y { 0 }; y < t_image.getSize().y; ++y)
    {
        if (t_image.getPixel(t_column, y).a != 0xFF)
        {
            return true;
        }
    }
    return false;
}

sf::Color getPixelWrapped(const sf::Image& t_image, sf::Vector2u t_pos)
{
    sf::Vector2u finalPos;
    finalPos.x = static_cast<unsigned>(t_image.getSize().x *
                                       (static_cast<double>(t_pos.x) / t_image.getSize().x - std::floor(static_cast<double>(t_pos.x) / t_image.getSize().x)));
    finalPos.y = static_cast<unsigned>(t_image.getSize().y *
                                       (static_cast<double>(t_pos.y) / t_image.getSize().y - std::floor(static_cast<double>(t_pos.y) / t_image.getSize().y)));

    return t_image.getPixel(finalPos.x, finalPos.y);
}

sf::Color bilinearFilter(const sf::Image& t_image, double t_u, double t_v)
{
    const unsigned x = static_cast<unsigned>(std::floor(t_u));
    const unsigned y = static_cast<unsigned>(std::floor(t_v));
    const double u_ratio = t_u - x;
    const double v_ratio = t_v - y;
    const double u_opposite = 1 - u_ratio;
    const double v_opposite = 1 - v_ratio;
    sf::Color result;
    result.r = static_cast<sf::Uint8>((getPixelWrapped(t_image, {x, y}).r * u_opposite + getPixelWrapped(t_image, {x + 1, y}).r * u_ratio) * v_opposite +
                                      (getPixelWrapped(t_image, {x, y + 1}).r * u_opposite + getPixelWrapped(t_image, {x + 1, y + 1}).r * u_ratio) * v_ratio);
    result.g = static_cast<sf::Uint8>((getPixelWrapped(t_image, {x, y}).g * u_opposite + getPixelWrapped(t_image, {x + 1, y}).g * u_ratio) * v_opposite +
                                      (getPixelWrapped(t_image, {x, y + 1}).g * u_opposite + getPixelWrapped(t_image, {x + 1, y + 1}).g * u_ratio) * v_ratio);
    result.b = static_cast<sf::Uint8>((getPixelWrapped(t_image, {x, y}).b * u_opposite + getPixelWrapped(t_image, {x + 1, y}).b * u_ratio) * v_opposite +
                                      (getPixelWrapped(t_image, {x, y + 1}).b * u_opposite + getPixelWrapped(t_image, {x + 1, y + 1}).b * u_ratio) * v_ratio);
    result.a = static_cast<sf::Uint8>((getPixelWrapped(t_image, {x, y}).a * u_opposite + getPixelWrapped(t_image, {x + 1, y}).a * u_ratio) * v_opposite +
                                      (getPixelWrapped(t_image, {x, y + 1}).a * u_opposite + getPixelWrapped(t_image, {x + 1, y + 1}).a * u_ratio) * v_ratio);
    if (result.a < 255)
    {
        result.a = 0;
    }

    return result;
}

void setBrightness(sf::Color& t_color, unsigned t_brightness)
{
    t_color.r = t_color.r * t_brightness / 255;
    t_color.g = t_color.g * t_brightness / 255;
    t_color.b = t_color.b * t_brightness / 255;
}

unsigned char getBrigthness(const sf::Color& t_color)
{
    return (t_color.r + t_color.g + t_color.b) / 3;
}

void slideImage(sf::Image &t_image, Side t_direction, size_t t_amount)
{
    sf::Image dest;
    dest.create(t_image.getSize().x, t_image.getSize().y, sf::Color::Transparent);
    if (t_direction == Side::South || t_direction == Side::West)
    {
        dest.copy(t_image, t_direction == Side::West ? t_amount : 0,
                  t_direction == Side::South ? t_amount : 0);
    }
    else
    {
        size_t x = t_direction == Side::East ? t_amount : 0;
        size_t y = t_direction == Side::North ? t_amount : 0;
        dest.copy(t_image, 0, 0, sf::Rect<int>(x, y,
                                               dest.getSize().x - x, dest.getSize().y - y));
    }
    t_image = dest;
}

sf::Image rotateImage90(const sf::Image &t_image)
{
    sf::Image rotated;
    rotated.create(t_image.getSize().y, t_image.getSize().x);

    for (unsigned i { 0 }; i < t_image.getSize().x; ++i)
    {
        for (unsigned j { 0 }; j < t_image.getSize().y; ++j)
        {
            rotated.setPixel(j, i, t_image.getPixel(i, j));
        }
    }

    return rotated;
}

std::vector<sf::Color> imageStripe(const sf::Image &t_image, unsigned t_y)
{
    assert(t_y < t_image.getSize().y);

    return std::vector<sf::Color>(t_image.getPixelsPtr() + t_image.getSize().x * t_y,
                                  t_image.getPixelsPtr() + t_image.getSize().x * (t_y + 1) - 1);
}

void createTextureArray(GLuint t_tex, const std::vector<sf::Image> &t_textures)
{
    assert(!t_textures.empty());

    auto size_x = std::max_element(t_textures.cbegin(), t_textures.cend(),
                                   [](const sf::Image& a, const sf::Image& b)
    {
        return a.getSize().x < b.getSize().x;
    })->getSize().x;

    auto size_y = std::max_element(t_textures.cbegin(), t_textures.cend(),
                                   [](const sf::Image& a, const sf::Image& b)
    {
        return a.getSize().y < b.getSize().y;
    })->getSize().y;

    glCheck(glActiveTexture(GL_TEXTURE0 + 31));

    glCheck(glBindTexture(GL_TEXTURE_2D_ARRAY, t_tex));

    glCheck(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, size_x, size_y, t_textures.size()));

    for (size_t i { 0 }; i < t_textures.size(); ++i)
    {
        glCheck(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, t_textures[i].getSize().x, t_textures[i].getSize().y,
                                1, GL_RGBA, GL_UNSIGNED_BYTE, t_textures[i].getPixelsPtr()));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glCheck(glTexParameterf(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR));
    glCheck(glTexParameterf(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE));
}

}
}
