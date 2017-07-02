#pragma once
#include "World.h"
#include "AnimationManager.h"
#include "Bullet.h"
#include <vector>

class weapon_class
{
	sf::Sprite					 Gun;

	bool						shooted,
								armed,
								reloading;

	int							bullets_count,
								clip_size,
								bullets_in_clip;

	sf::Time					shoot_time,
								reload_time;

	sf::Sprite					&body;
	std::vector<sf::Sound>		WeaponSound;
	enum{
		shot_,
		cock_,
		reload_
	};
public:

	static std::vector<Bullet*> bullets;

	enum Gun_type
	{
		rifle,
		laser_minigun,
	} ThisGun;

public:

	weapon_class(sf::Sprite &playerPos);
	std::pair<int, int> GetBulletsInfo();
	void				setCurWeapon(sf::Texture& texture, Gun_type pGun);
	void				setSound(SoundHolder& snd);
	void				DefineWeapon();
	int&				BulletsCount();
	void				rotate(float rotation);
	void				SetPosition(sf::Vector2f pos);
	void				shot(tmx::MapLoader& mlvl, sf::Texture& ptexture, sf::Vector2f Playerpos);
	bool				GetShooted();
	bool				IsArmed();
	void				update_bullet(sf::Time time);
	void				ArmsAnimation(sf::Time time);
	void				draw(sf::RenderWindow& W);
};