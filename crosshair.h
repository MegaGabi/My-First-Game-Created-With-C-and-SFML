#pragma once
#include "World.h"

class crosshair
{
	sf::Sprite cross;

public:
	crosshair()
	{}

	void SetTexture(sf::Texture& ptxt)
	{
		cross.setTexture(ptxt);
		cross.setOrigin(cross.getGlobalBounds().width / 2, cross.getGlobalBounds().height / 2);
	}

	void update(sf::RenderWindow& W)
	{
		cross.setPosition(sf::Vector2f(W.mapPixelToCoords(sf::Mouse::getPosition(W), W.getView())));
	}

	void draw(sf::RenderWindow& W)
	{
		W.draw(cross);
	}

	void SetColor(sf::Color clr)
	{
		cross.setColor(clr);
	}

	sf::Vector2f GetPosition()
	{
		return cross.getPosition();
	}
};

