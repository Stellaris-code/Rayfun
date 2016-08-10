/*  src/animatedbillboard.cpp AnimatedBillboard - Yann BOUCHER (yann) 17/06/2016
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
#include "animatedbillboard.hpp"

#include "mathutility.hpp"
#include <iostream>

namespace Rayfun
{

AnimatedBillboard::AnimatedBillboard(State::Context &t_context) : DrawableActor(t_context)
{
    m_img.create(1, 1, sf::Color::Transparent);
}

void AnimatedBillboard::setSpriteSheet(const sf::Image &t_img)
{
    m_spriteSheet = t_img;
}

void AnimatedBillboard::addFrame(const sf::Time &t_animTime, const sf::IntRect &t_rect, double t_minAngle,
                                 double t_maxAngle)
{
    //m_frameQueue.push_back({{Utility::normalizeAngle(t_minAngle), Utility::normalizeAngle(t_maxAngle)}, {t_animTime, t_rect}});
    t_minAngle = Utility::normalizeAngle(t_minAngle);
    t_maxAngle = Utility::normalizeAngle(t_maxAngle);
    for (auto& pair : m_frameQueue)
    {
        if (Utility::nearEqual(pair.first.first, t_minAngle) && Utility::nearEqual(pair.first.second, t_maxAngle))
        {
            pair.second.push_back({t_animTime, t_rect});
            goto found;
        }
    }
    m_frameQueue.push_back({{t_minAngle, t_maxAngle}, {{t_animTime, t_rect}}});

    found:
    if (m_frameQueue.size() == 1)
    {
        m_img.create(m_frameQueue.front().second.front().rect.width, m_frameQueue.front().second.front().rect.height, sf::Color::Transparent);
        m_img.copy(m_spriteSheet, 0, 0, m_frameQueue.front().second.front().rect);
    }
}

void AnimatedBillboard::update(const sf::Time &t_deltaTime)
{
    if (!angleToFrame(playerFacingAngle()).empty())
    {
        m_progress += t_deltaTime;
        if (m_progress >= currentFrame().animTime)
        {
            ++m_currentFrame;
            if (m_currentFrame >= angleToFrame(playerFacingAngle()).size())
            {
                m_currentFrame = 0;
            }
            m_img.create(currentFrame().rect.width, currentFrame().rect.height, sf::Color::Transparent);
            m_img.copy(m_spriteSheet, 0, 0, currentFrame().rect, true);
            m_progress = sf::Time::Zero;
        }
    }
}

sf::Image AnimatedBillboard::renderImage() const
{
    return m_img;
}

std::vector<AnimatedBillboard::Frame>& AnimatedBillboard::angleToFrame(double t_angle)
{
    for (auto& pair : m_frameQueue)
    {
        if (t_angle >= pair.first.first && t_angle <= pair.first.second)
        {
            return pair.second;
        }
    }
}

AnimatedBillboard::Frame &AnimatedBillboard::currentFrame()
{
    return angleToFrame(playerFacingAngle())[m_currentFrame];
}

}
