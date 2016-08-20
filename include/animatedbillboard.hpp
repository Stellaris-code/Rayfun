/*  include/animatedbillboard.hpp AnimatedBillboard - Yann BOUCHER (yann) 17/06/2016
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
#ifndef ANIMATEDBILLBOARD_HPP
#define ANIMATEDBILLBOARD_HPP

#include <vector>

#include <SFML/Graphics/Image.hpp>

#include "drawableactor.hpp"

namespace Rayfun
{

class AnimatedBillboard : public DrawableActor
{

    public:
        explicit AnimatedBillboard(State::Context& t_context);
        virtual ~AnimatedBillboard() {}

    public:
        void setSpriteSheet(const sf::Image& t_img);
        void addFrame(const sf::Time& t_animTime, const sf::IntRect& t_rect, double t_minAngle = 0,
                      double t_maxAngle = 360);

    public:
        virtual void update(const sf::Time& t_deltaTime) override;
        virtual sf::Image renderImage() const override;

    private:
        struct Frame
        {
                sf::Time animTime;
                sf::IntRect rect;
        };

    private:
        std::vector<Frame>& angleToFrame(double t_angle);
        Frame& currentFrame();

    private:
        sf::Image m_spriteSheet;
        sf::Image m_img;
        sf::Time m_progress;
        size_t m_currentFrame { 0 };
        std::vector<Frame> m_defaultFrame { { sf::seconds(0.1), {0, 0, 1, 1} } };
        std::vector<std::pair<std::pair<double, double>, std::vector<Frame>>> m_frameQueue;
};

}

#endif // ANIMATEDBILLBOARD_HPP
