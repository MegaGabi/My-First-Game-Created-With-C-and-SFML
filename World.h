#pragma once
#include "helper/ResourceHolder.hpp"
#include "helper/ResourceIdentifiers.hpp"

#include <tmx/MapLoader.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>

const sf::Time	TimePerFrame = sf::seconds(1.f / 60.f);

class World
{
public:
	TextureHolder						mTextures;
	SoundHolder							mSounds;
	tmx::MapLoader						mlvl;
public:
	static std::vector<sf::RectangleShape>		cells;

										World(sf::RenderWindow& window);
	void								draw();
	void								setView(sf::Vector2f a);
	//void								plusView(sf::Vector2f a);
	void								ConfigureView();
	void								ChangeResolution(sf::FloatRect a);
	void								loadMAP();
	//sf::View							getView();
private:
	void								loadTextures();
	void								loadSounds();

private:
	enum Lvl_type
	{
		none,
		test,
		end,
		Count_of_levels
	}curr_lvl;
	bool next_map;

public:
	sf::RenderWindow&					mWindow;
	sf::View							mWorldView;
	sf::FloatRect						mWorldBounds;
	
};

