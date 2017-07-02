#include "World.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

std::vector<sf::RectangleShape>	World::cells;

World::World(sf::RenderWindow& window)
: mWindow(window),
mWorldView(window.getDefaultView()),
mTextures(),
mSounds(),
mlvl("Media\\maps\\"),
next_map(1),
curr_lvl(none)
{
	mWorldView.reset(sf::FloatRect(0, 0, 640, 480));
	mWindow.setView(mWorldView);
	loadTextures();
	loadSounds();
	loadMAP();
	
	for (int i = 0; i < (mlvl.GetMapSize().y / 32); i++)
	{
		std::vector<sf::RectangleShape> temp;
		for (int j = 0; j < (mlvl.GetMapSize().x / 32 + 1); j++)
		{
			temp.push_back(sf::RectangleShape(sf::Vector2f(32, 32)));
			temp[temp.size()-1].setPosition
				(
				sf::Vector2f((j * 32), (i * 32))
				);
			temp[temp.size() - 1].setFillColor(sf::Color::Transparent);
			temp[temp.size() - 1].setOutlineThickness(3);
			temp[temp.size() - 1].setOutlineColor(sf::Color::Red);
		}
		cells.insert(std::end(cells), std::begin(temp), std::end(temp));
	}

}

void World::draw()
{
	mWindow.draw(mlvl);
}

void World::setView(sf::Vector2f a)
{
	mWorldView.setCenter(a.x, a.y);
}

void  World::ConfigureView()
{
	mWindow.setView(mWorldView);
}

void World::ChangeResolution(sf::FloatRect a)
{
	mWorldView.reset(a);
}

void World::loadTextures()
{
	mTextures.load(Textures::crosshair, "Media/images/cross/crosshair.png");

	mTextures.load(Textures::Player_body, "Media/images/player/body.png");
	mTextures.load(Textures::Player_legs, "Media/images/player/legs/legs.png");

	mTextures.load(Textures::Dummy, "Media/images/enemies/training_dummy.png");
	mTextures.load(Textures::crab, "Media/images/enemies/dustcrab/crab.png");

	mTextures.load(Textures::arms, "Media/images/guns/rifle/s_rifle.png"); //стандартное оружие
	mTextures.load(Textures::bullet, "Media/images/guns/rifle/bullet.png");//стандартные пули
	mTextures.load(Textures::ammo, "Media/images/guns/ammo_crate.png");

	mTextures.load(Textures::button_ark, "Media/images/menu/button_ark.png");

	mTextures.load(Textures::hood_texture, "Media/images/hood/hood.png");
	mTextures.load(Textures::hood_texture_fill, "Media/images/hood/fill.png");

	mTextures.load(Textures::inventory_animation, "Media/images/inventory/animation_inventory.png");
	mTextures.load(Textures::inventory_face, "Media/images/inventory/inventory.png");
	mTextures.load(Textures::inventory_armour_info, "Media/images/inventory/invArmourinfo.png");
	mTextures.load(Textures::inventory_list_grenade, "Media/images/inventory/invGrenadeandShieldlist.png");
	mTextures.load(Textures::inventory_list_weapons, "Media/images/inventory/invWeaponlist.png");
	mTextures.load(Textures::inventory_marker, "Media/images/inventory/invGrenadeandShiedMarcedslot.png");
	mTextures.load(Textures::inventory_icons, "Media/images/inventory/icons.png");
	mTextures.load(Textures::inventory_weapon_slots, "Media/images/inventory/invWeaponSlots.png");
}

void World::loadSounds()
{
	mSounds.load(Sounds::background, "Media/sound/back.wav");
	mSounds.load(Sounds::shot, "Media/sound/single_shot.wav");
	mSounds.load(Sounds::cock, "Media/sound/rifle_cock.wav");
	mSounds.load(Sounds::reload, "Media/sound/reloading.wav");
	mSounds.load(Sounds::step, "Media/sound/steps_player.wav");
	mSounds.load(Sounds::dying_monster, "Media/sound/monster_die.wav");
	mSounds.load(Sounds::onButton, "Media/sound/OnButton.wav");
	mSounds.load(Sounds::pressButton, "Media/sound/PressButton.wav");

}


void World::loadMAP()
{
	if (next_map)
	{
		switch (curr_lvl)
		{
		case none:
		{
					 mlvl.Load("stage1.tmx");
					 curr_lvl = test;
					 break;
		}
		case test:
		{
					 //какая-то карта
		}
		}
		next_map = false;
	}
}