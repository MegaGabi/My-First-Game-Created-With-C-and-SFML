#pragma once
#include "AnimationManager.h"
#include "World.h"			

const sf::Vector2f		posToDrawWeaponList(282, 18),
						posToDrawGrenadeList(322, 18),
						posToDrawIcons(28, 2),
						posToDrawWeaponSlot(/*38*/0, 78),
						posFirstMarker(230,92),
						posSecondMarker(230, 204),
						posToDrawArmorInfo(167,78);

const sf::FloatRect		gun_s_icon(sf::Vector2f(posToDrawIcons), sf::Vector2f(72, 30)),
						grenade_icon(108, 2, 22, 30),
						shield_icon(148, 2, 30, 30),
						armor_icon(190, 2, 50, 30),
						up_arrow(342, 24, 20, 18),
						down_arrow(342, 348, 20, 18);

const sf::Time			animation_time(sf::seconds(0.25));

class inventory
{
private:
	AnimationManager	inventory_animation;
	sf::Sprite			inventory_face,
						marker,
						weapon_slots,
						list_grenade_and_shield,
						list_weapon,
						armour_info,
						icon,
						choosed_icon;

	bool				isShown,
						isAnimation;

	enum {weapon, grenade, shield, armour} current_menu;
public:
	inventory();
	void start();
	void stop();
	void setTextures(TextureHolder& pTxtrs);
	void check_if_on();
	bool& isInventory();
	void animate(sf::Time t);
	void update(sf::Vector2f pos, sf::RenderWindow& W);
	void Draw(sf::Vector2f pos, sf::RenderWindow& mWind);
};

