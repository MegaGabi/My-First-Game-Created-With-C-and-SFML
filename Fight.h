#pragma once
#include "Player.h"
#include "Enemy.h"
#include "weapon_class.h"
#include "drop_ammo.h"

class Fight
{
	Player&					pPlayer;
	std::vector<Bullet*>&	bullets;
	std::vector<Enemy>&		Enemies;
	std::vector<drop_ammo>	ammo;
public:
	Fight(Player& pPlayer, std::vector<Enemy>& Enemies) :pPlayer(pPlayer), Enemies(Enemies), bullets(pPlayer.Gun.begin()->second.bullets)
	{
	}

	void FightDaPower(sf::Texture& drop)
	{
		//<color and killing>
		pPlayer.cross.SetColor(sf::Color::Green);

		for (int i = 0; i < ammo.size();)
		{
			if (ammo[i].crate.getGlobalBounds().intersects(pPlayer.GetPlayerRect()))
			{
				pPlayer.Gun.at(pPlayer.CurrGun).BulletsCount() += ammo[i].value;
				ammo.erase(ammo.begin() + i);
			}
			else
			{
				i++;
			}
		}

		for (int i = 0; i < Enemies.size(); i++)
		{
			if (Enemies[i].GetRect().contains(pPlayer.cross.GetPosition()))
			{
				if (Enemies[i].GetHealth() > 0)
					pPlayer.cross.SetColor(sf::Color::Red);
			}
			for (int j = 0; j < bullets.size(); j++)
			{
				if (Enemies[i].GetRect().intersects(bullets[j]->GetRect()) && Enemies[i].GetHealth() > 0)
				{
					Enemies[i].GetHealth() -= 50;
					bullets.erase(bullets.begin() + j);
					if (Enemies[i].GetHealth() <= 0)
					{
						pPlayer.Score += 20 + rand() % 20;
						if (rand()%10 == 0)
							ammo.push_back(drop_ammo(sf::Vector2f(Enemies[i].GetRect().left, Enemies[i].GetRect().top), drop));
					}
				}
			}
		}
		//</color and killing>
	}

	void Draw(sf::RenderWindow& W)
	{
		for (auto i : ammo)
			i.draw(W);
	}
};

