#pragma once
#include "Entity.h"
#include <SFML/Audio.hpp>
#include <cmath>

class Bullet:public Entity
{
public:
	sf::Sprite body;
	float speed;
	Bullet(tmx::MapLoader& plvl, sf::Texture& ptxt, sf::Vector2f Playerpos, int rotation);
	sf::FloatRect GetRect();
	void move_bullet(float time);
	bool kill_bullet();
};

