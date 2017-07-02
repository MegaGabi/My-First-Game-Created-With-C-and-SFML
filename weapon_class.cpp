#include "weapon_class.h"

std::vector<Bullet*> weapon_class::bullets;

weapon_class::weapon_class(sf::Sprite &playerPos) :shoot_time(sf::seconds(0)), reload_time(sf::seconds(2)), shooted(0), body(playerPos),
reloading(0)
{
}

std::pair<int, int> weapon_class::GetBulletsInfo()
{
	return std::pair<int, int>(bullets_in_clip, bullets_count/clip_size);
}

void weapon_class::setCurWeapon(sf::Texture& texture, Gun_type pGun)
{
	ThisGun = pGun;
	Gun.setTexture(texture);
	Gun.setTextureRect(sf::IntRect(0, 0, 32, 32));
	Gun.setOrigin(Gun.getGlobalBounds().width / 2, Gun.getGlobalBounds().height + 3);
	armed = true;
	DefineWeapon();
}

void weapon_class::setSound(SoundHolder& snd)
{
	WeaponSound.resize(3);
	WeaponSound[shot_].setBuffer(snd.get(Sounds::ID::shot));
	WeaponSound[cock_].setBuffer(snd.get(Sounds::ID::cock));
	WeaponSound[reload_].setBuffer(snd.get(Sounds::ID::reload));
}

void weapon_class::DefineWeapon()
{
	if (ThisGun == Gun_type::rifle)
	{
		bullets_in_clip = clip_size = 30;
		bullets_count = 300;
	}
	else if (ThisGun == Gun_type::laser_minigun)
	{
		bullets_in_clip = clip_size = 100;
		bullets_count = 300;
	}
}

int& weapon_class::BulletsCount()
{
	return bullets_count;
}

void weapon_class::rotate(float rotation)
{
	Gun.setRotation(rotation);
}

void weapon_class::SetPosition(sf::Vector2f pos)
{
	Gun.setPosition(pos);
}

void weapon_class::update_bullet(sf::Time time)
{
	for (unsigned i = 0; i<bullets.size(); i++)
	{
		bullets.at(i)->move_bullet(time.asMilliseconds());
		if (bullets.at(i)->kill_bullet())
			bullets.erase(bullets.begin() + i);
	}
}

void weapon_class::shot(tmx::MapLoader& mlvl, sf::Texture& ptexture, sf::Vector2f Playerpos)
{
	if (bullets_in_clip)
	{
		WeaponSound[shot_].play();
		shooted = true;
		bullets.push_back(new Bullet(mlvl, ptexture, Playerpos, Gun.getRotation()));
		Gun.setTextureRect(sf::IntRect(32, 0, 32, 32));
		bullets_in_clip--;
	}
	else if (bullets_count >= clip_size)
	{
		if (WeaponSound[reload_].getStatus() == sf::Sound::Stopped)
			WeaponSound[reload_].play();
		bullets_count -= clip_size;
		bullets_in_clip = clip_size;
		reloading = true;
	}
	else if (bullets_count)
	{
		if (WeaponSound[reload_].getStatus() == sf::Sound::Stopped)
			WeaponSound[reload_].play();
		bullets_in_clip = bullets_count;
		bullets_count = 0;
		reloading = true;
	}
	else
	{
		if (WeaponSound[cock_].getStatus() == sf::Sound::Stopped)
			WeaponSound[cock_].play();
	}
}

//возвращает переменную, которая указывает произошел выстрел или нет
bool weapon_class::GetShooted()
{
	return shooted;
}

bool weapon_class::IsArmed()
{
	return armed;
}

void weapon_class::ArmsAnimation(sf::Time time)
{
	if (shooted && !reloading)
	{
		if (shoot_time >= sf::seconds(0.05))
		{
			Gun.setTextureRect(sf::IntRect(0, 0, 32, 32));
			float x_pos = cos((Gun.getRotation() - 90)* 3.14 / 180)*0.5;
			float y_pos = sin((Gun.getRotation() - 90)* 3.14 / 180)*0.5;
			Gun.move(-x_pos, -y_pos);
		}

		if (shoot_time >= sf::seconds(0.1))
		{
			shooted = false;
			shoot_time = sf::Time::Zero;
			Gun.setPosition(body.getPosition());
		}
		
	}
	else if (reloading)
	{
		Gun.setTextureRect(sf::IntRect(0, 0, 32, 32));
		if (shoot_time >= reload_time)
			reloading = false;
	}

	shoot_time += time;
}

void weapon_class::draw(sf::RenderWindow& W)
{
	W.draw(Gun);
	for (unsigned i = 0; i<bullets.size(); i++)
	{
		W.draw(bullets.at(i)->body);
	}
}
