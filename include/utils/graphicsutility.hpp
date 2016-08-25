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
#ifndef GRAPHICSUTILITY_HPP
#define GRAPHICSUTILITY_HPP

#include <cstddef>

#include <vector>

#include <SFML/OpenGL.hpp>


namespace sf
{
class Image;
class Texture;
template <typename T>
class Vector2;
class Color;
typedef Vector2<float> Vector2f;
typedef Vector2<unsigned> Vector2u;
typedef Vector2<size_t> Vector2s;
}

namespace Rayfun
{

enum class Side;

namespace Utility
{

template <class T>
void setOriginToCenter(T& t_target)
{
    auto rect = t_target.getLocalBounds();
    t_target.setOrigin(rect.left + rect.width / 2.f,
                       rect.top  + rect.height / 2.f);
}

void addDecal(sf::Image &t_target, sf::Image t_decal, const sf::Vector2s &t_pos, bool t_randomFlip = false);

bool imageContainsAlpha(const sf::Image& t_image);
bool imageStripContainsAlpha(const sf::Image& t_image, unsigned t_column);

sf::Image rotateImage90(const sf::Image& t_image);

std::vector<sf::Color> imageStripe(const sf::Image& t_image, unsigned t_y);

sf::Color getPixelWrapped(const sf::Image& t_image, sf::Vector2u t_pos);

sf::Color bilinearFilter(const sf::Image& t_image, double t_u, double t_v);

void setBrightness(sf::Color &t_color, unsigned t_brightness);
unsigned char getBrigthness(const sf::Color& t_color);

void slideImage(sf::Image& t_image, Side t_direction, size_t t_amount);

void createTextureArray(GLuint t_tex, const std::vector<sf::Image>& t_textures);

}
}

#endif // GRAPHICSUTILITY_HPP
