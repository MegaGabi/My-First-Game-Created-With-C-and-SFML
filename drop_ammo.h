#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class drop_ammo{
public:
	int value;
	sf::Sprite crate;

	drop_ammo(sf::Vector2f pos, sf::Texture& txt)
	{
		crate.setTexture(txt);
		crate.setPosition(pos);
		value = (rand() % 2  + 1)*30;
	}

	void draw(sf::RenderWindow& W)
	{
		W.draw(crate);
	}
};