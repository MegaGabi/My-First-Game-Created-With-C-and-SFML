#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "world.h"

class Entity
{
public:
	int					live;
	tmx::MapLoader&		mlvl;
public:
						Entity(tmx::MapLoader& plvl);
	virtual void		StandartMapCollisison(float DX, float DY, float& x, float& y, /*tmx::MapObject mEntityObj*/sf::FloatRect rct);

private:
	sf::Vector2f		mVelocity;
};

