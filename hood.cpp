#include "hood.h"
#include "Player.h"

hood::hood()
{}

void hood::SetTexture(TextureHolder& ptxt)
{
	hood_background.setTexture(ptxt.get(Textures::hood_texture));
	for (int i = 0; i < health + 1; i++)
	{
		cover_up[i].setTexture(ptxt.get(Textures::hood_texture_fill));
	}
}

sf::Vector2f hood::GetPosition()
{
	return sf::Vector2f(
		hood_background.getPosition().x + hood_background.getGlobalBounds().width / 2,
		hood_background.getPosition().y + hood_background.getGlobalBounds().height);//центр нижнего
}

void hood::update(sf::Vector2f win_size, Player *pPlayer)
{
	sf::Vector2f NewPosition = 
		pPlayer->GetPlayerPosititon() + 
		sf::Vector2f(-hood_background.getGlobalBounds().width / 2,
		win_size.y / 2 - hood_background.getGlobalBounds().height*1.3);

		hood_background.setPosition(NewPosition);
	for (int i = 0; i < health + 1; i++)
	{
		cover_up[i].setPosition(hood_background.getPosition() + sf::Vector2f(97, 22) + sf::Vector2f(i * (-8), i * (14)));
		//^позиция заполнителей статус баров относительно бэкграунда худа
	}

	cover_up[armor].setScale(-1 + float(pPlayer->GetArmor()) / 100, 1);
	cover_up[health].setScale(-1 + float(pPlayer->GetHealth()) / 100, 1);

}

void hood::DrawHood(sf::RenderWindow& mWind)
{
	mWind.draw(hood_background);
	for (int i = 0; i < health + 1; i++)
	{
		mWind.draw(cover_up[i]);
	}
}
