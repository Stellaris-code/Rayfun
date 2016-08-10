/*  include/weapon.hpp Weapon - Yann BOUCHER (yann) 19/06/2016
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
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <functional>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <Thor/Animations/FrameAnimation.hpp>
#include <Thor/Animations/AnimationMap.hpp>
#include <Thor/Animations/Animator.hpp>

#include "common.hpp"

namespace Rayfun
{

class Weapon : public sf::Drawable, public sf::Transformable
{
    public:
        Weapon(unsigned long& t_ammopool, long t_magazineSize);
        virtual ~Weapon() = default;

    public:
        void setSpriteSheet(const sf::Texture& t_spriteSheet);
        void setFireAnimation(const thor::FrameAnimation& t_anim,
                              const sf::Time& t_duration);
        void setReloadAnimation(const thor::FrameAnimation& t_anim,
                                const sf::Time& t_duration);
        void setIdleAnimation(const thor::FrameAnimation& t_anim,
                                const sf::Time& t_duration);

        void setFireCallback(const std::function<void()>& t_callback);
        void setReloadCallback(const std::function<void()>& t_callback);

        void setResources(const Resources& t_res)
        {
            m_res = &t_res;
        }

        void reload();
        void fire();

        virtual void update(const sf::Time& t_deltaTime);

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const override;

    public:
        std::string name;

    private:
        void reloadMagazine();
        void fireAmmo();
        bool checkCanFire();
        bool checkCanReload();

    private:
        sf::Sprite m_spriteSheet;
        Resources const* m_res { nullptr };
        unsigned long& m_ammoPool;
        const long m_magazineSize { 0 };
        long m_ammoInMagazine { m_magazineSize };
        bool m_animPlaying { false };
        bool m_fireAnimSet { false };
        bool m_reloadAnimSet { false };
        bool m_idleAnimSet { false };
        std::function<void()> m_fireCallback;
        std::function<void()> m_reloadCallback;
        thor::AnimationMap<sf::Sprite, std::string> m_anims;
        thor::Animator<sf::Sprite, std::string> m_animator;
};

}

#endif // WEAPON_HPP
