#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "World.h"
#include "main_menu.h"
#include "Fight.h"

const sf::Color ORANGE(255, 106, 0);	//объ€вление цвета отсутствоющего в библиотеке sfml

class Game
{
private:
	sf::RenderWindow		mWindow;
	std::vector<Enemy>		Enemies;
	int						MaxEnemies;
	main_menu				mMenu;
	main_menu				subMenu;
	main_menu				DeathScreen;
	bool					IsInMainMenu;
	bool					IsInSubMenu;
	World					mWorld;
	Player					mPlayer;
	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Text				ScoreInfo;
	sf::Time				mStatisticsUpdateTime;
	sf::Sound				pBackground;
	std::vector<sf::Sound>  Menu_Sounds;
	std::size_t				mStatisticsNumFrames;
	Fight					mFight;

	enum Sounds_Menu
	{
		onButton,
		pressButton
	};
public:
	Game();
	void run();
private:
	void					processInput();
	void					MainMenuInteractions(sf::Event evnt);
	void					SubMenuInteractions(sf::Event evnt);
	void					DeathScreenInteractions(sf::Event evnt);
	void					update(sf::Time elapsedTime);
	void					render();
	void					updateStatistics(sf::Time elapsedTime);
};

