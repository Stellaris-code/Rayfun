/*  src/melttransition.cpp MeltTransition - Yann BOUCHER (yann) 13/08/2016
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
#include "screenmelt.hpp"

#include <algorithm>
#include <random>
#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>


void MeltTransition::reset(const sf::Texture &t_tex, sf::Time t_animDuration, unsigned t_stripeSize)
{
    m_tex = &t_tex;
    m_animDuration = t_animDuration * 0.8f;
    m_stripeSize = t_stripeSize;

    m_stripes.clear();
    m_completedMap.clear();
    m_delayMap.clear();
    createStripes();
}

void MeltTransition::update(sf::Time t_deltaTime)
{
    checkTextureIsValid();
    if (!completed())
    {
#pragma omp parallel for simd
        for (size_t i = 0; i < m_delayMap.size(); ++i)
        {
            auto& delay = m_delayMap[i];
            auto& stripe = m_stripes[i];
            delay -= t_deltaTime.asSeconds();

            if (!m_completedMap[i] && delay <= 0)
            {
                stripe.move(0, m_tex->getSize().y * (t_deltaTime / m_animDuration));
                if (stripe.getPosition().y > m_tex->getSize().y)
                {
                    stripe.setPosition(stripe.getPosition().x, m_tex->getSize().y);
                    m_completedMap[i] = true;
                }
            }
        }
    }
}

bool MeltTransition::completed() const
{
    return std::all_of(m_completedMap.cbegin(), m_completedMap.cend(), [](bool b){ return b; });
}

void MeltTransition::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    checkTextureIsValid();
    t_states.transform *= getTransform();

    for (const auto& stripe : m_stripes)
    {
        t_target.draw(stripe, t_states);
    }
}

void MeltTransition::createStripes()
{
    checkTextureIsValid();

    m_stripes.resize(m_tex->getSize().x / m_stripeSize);
    m_delayMap.resize(m_stripes.size());
    m_completedMap.resize(m_stripes.size(), false);
    std::default_random_engine generator;

#pragma omp parallel for simd
    for (size_t i = 0; i < m_stripes.size(); ++i)
    {
        m_stripes[i].setTexture(*m_tex);
        m_stripes[i].setTextureRect(sf::IntRect(i * m_stripeSize, 0, m_stripeSize, m_tex->getSize().y));
        m_stripes[i].setPosition(i * m_stripeSize, 0);

        if (i == 0)
        {
            std::uniform_real_distribution<float> distribution(0.f, 0.25f * m_animDuration.asSeconds());
            m_delayMap[i] = distribution(generator);
        }
        else
        {
            // value based on neighbor; for rolling hill effect
            std::uniform_real_distribution<float> distribution(
                        m_delayMap[i - 1] - 0.05f * m_animDuration.asSeconds(),
                    m_delayMap[i - 1] + 0.05f * m_animDuration.asSeconds());

            m_delayMap[i] = distribution(generator);
            if (m_delayMap[i] < 0)
            {
                m_delayMap[i] = std::abs(m_delayMap[i] / 2);
            }
            else if (m_delayMap[i] > 0.25f * m_animDuration.asSeconds())
            {
                m_delayMap[i] = 0.25f * m_animDuration.asSeconds();
            }
        }
    }
}

void MeltTransition::checkTextureIsValid() const
{
    if (m_tex == nullptr)
    {
        throw_screenmelt_error("Tried to do screen melt with a null texture !");
    }
}
