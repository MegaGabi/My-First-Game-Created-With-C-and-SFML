#pragma once
#include "Entity.h"
#include "inventory.h"
#include "hood.h"
#include "Bullet.h"
#include "AnimationManager.h"
#include "weapon_class.h"
#include "World.h"
#include "crosshair.h"
#include <vector>

class Player:public Entity
{
private:
	sf::RenderWindow&		W;
	sf::Text				mStatisticsText;
	sf::Time				shoot_time;
	sf::Sprite				sprite_body;
	AnimationManager		animation_legs;
	inventory				mInventory;
	hood					mHood;//TEST!!!ACHTUNG!!!
	float					speed;

	float					dx, 
							dy;

	float					x,
							y;

	int						armor;

	std::vector<sf::Sound>	plrSounds;

	enum
	{
		snd_step,
	};
public:
	int													 Score;
	crosshair											 cross;

	std::map<weapon_class::Gun_type ,weapon_class>		 Gun;

	weapon_class::Gun_type								 CurrGun;

	enum Action
	{
		MoveUp    = 0,
		MoveRight = 90,
		MoveDown  = 180,
		MoveLeft  = 270
	};

	tmx::MapObject PlayerObject;

public:
							Player(tmx::MapLoader& plvl, sf::RenderWindow& W, sf::Font& mFont);
	void					setTexture(TextureHolder& ptexture);
	void					setObject();
	void					setSound(SoundHolder& snd);
	sf::Sprite				GetBodySprite();
	void					rotation(sf::Vector2i pM_pos);
	void					update(Action button, float time);
	void					second_update(sf::Time t, World& mWorld);
	void					drawPlayer(sf::RenderWindow& W);
	bool					IsInInventory();
	sf::Vector2f			GetPlayerPosititon();
	sf::FloatRect			GetPlayerRect();
	int&					GetArmor();
	int&					GetHealth();
	void					Animate(const sf::Time& time);
};

