#include "Bullet.h"

Bullet::Bullet(tmx::MapLoader& plvl, sf::Texture& ptxt, sf::Vector2f Playerpos, int rotation) :Entity(plvl)
{
	float radius = 1;
	speed = 1.5;
	live = 1;
	body.setTexture(ptxt);
	float x_pos = Playerpos.x + (cos((rotation - 90)* 3.14 / 180) * radius);
	float y_pos = Playerpos.y + (sin((rotation - 90)* 3.14 / 180) * radius);
	body.setPosition(x_pos, y_pos);
	body.setOrigin(body.getGlobalBounds().width / 2, body.getGlobalBounds().height / 2);
	body.setRotation(rotation);
}

sf::FloatRect Bullet::GetRect()
{
	return body.getGlobalBounds();
}

void Bullet::move_bullet(float time)
{
	float x = speed * time * cos(((body.getRotation() - 90)* 3.14 / 180));
	float y = speed * time * sin(((body.getRotation() - 90)* 3.14 / 180));
	body.move(x, y);
}

bool Bullet::kill_bullet()
{
	for (auto layer = mlvl.GetLayers().begin(); layer != mlvl.GetLayers().end(); ++layer)
	{
		if (layer->name == "objects")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
			{
				if (object->GetName() == "solid")
				{
					if (object->GetAABB().intersects(body.getGlobalBounds()))
					{
						return true;
					}
				}
			}
		}
	}
	if ((body.getPosition().x > mlvl.GetMapSize().x || body.getPosition().x < 0)
		||
		(body.getPosition().y > mlvl.GetMapSize().y || body.getPosition().y < 0))
	{
		std::cout << "OUT!";
		return true;
	}
	return false;
}
