#pragma once
#include <SFML/Graphics.hpp>
//#include "Player.h"
#include "helper/ResourceHolder.hpp"
#include "helper/ResourceIdentifiers.hpp"

class Player;

class hood
{
	sf::Sprite hood_background;
	sf::Sprite cover_up[2];
	sf::Vector2f wSize;//size of window
	enum
	{
		armor,
		health		
	};
public:
	hood();
	void SetTexture(TextureHolder& ptxt);//hood_background
	sf::Vector2f GetPosition();
	void update(sf::Vector2f win_size, Player *pPlayer);
	void DrawHood(sf::RenderWindow& mWind);
};

