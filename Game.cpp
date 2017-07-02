#include "Game.h"
#include "helper/StringHelpers.hpp"

enum {mresume, mtoMainMenu, mtoDesktop};//subMenu
enum {mstart, mexit};//mMenu
enum {drestart, dexit};//death_screen

Game::Game() :
mWindow(sf::VideoMode(640, 480), "Game [Test Version] v0.0001 pre-pre-Alpha"/*, sf::Style::Fullscreen*/),
mFont(),
mStatisticsText(),
mStatisticsUpdateTime(),
mStatisticsNumFrames(0),
mWorld(mWindow),
mPlayer(mWorld.mlvl, mWindow, mFont),
subMenu(/*sf::Vector2f(170, 170)*/sf::Vector2f(mWindow.getDefaultView().getCenter().x / 2, mWindow.getDefaultView().getCenter().y / 2)),
mMenu(sf::Vector2f(mWindow.getDefaultView().getCenter().x / 2, mWindow.getDefaultView().getCenter().y / 2)),
DeathScreen(sf::Vector2f(mWindow.getDefaultView().getCenter().x / 2, mWindow.getDefaultView().getCenter().y / 2)),
IsInMainMenu(true),
IsInSubMenu(false),
mFight(mPlayer, Enemies),
MaxEnemies(15)
{
	mWindow.setVerticalSyncEnabled(true);
	for (int i = 0; i < MaxEnemies; i++)//test_enemy
	{
			Enemies.push_back(Enemy(mWorld, mWindow, mPlayer, Enemies));//test_enemy
			Enemies[Enemies.size() - 1].SetTextures(mWorld.mTextures, Textures::ID::crab);//test_enemy
	}

	/*mWindow.setMouseCursorVisible(0);*/
	mFont.loadFromFile("Media/Bricks.otf");

	mStatisticsText.setFont(mFont);
	mStatisticsText.setCharacterSize(10);
	mStatisticsText.setPosition(5.f, 5.f);

	ScoreInfo.setFont(mFont);
	ScoreInfo.setCharacterSize(50);
	ScoreInfo.setColor(sf::Color::Red);
	ScoreInfo.setPosition(mWindow.getDefaultView().getCenter().x / 2, mWindow.getDefaultView().getCenter().y / 2 - 50);
	ScoreInfo.setString("Points: " + toString(mPlayer.Score));

	mPlayer.setObject();
	mPlayer.setTexture(mWorld.mTextures);

	mMenu.SetButton(int(mstart), L"\t  Начать Игру "); /*Start New Game*/
	mMenu.SetButton(int(mexit),  L"\t          Выйти");
	mMenu.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));

	subMenu.SetButton(int(mresume),     L"\t      Продолжить  ");
	subMenu.SetButton(int(mtoMainMenu), L"\t    В Главное Меню");
	subMenu.SetButton(int(mtoDesktop),  L"\t   На Рабочий стол");
	subMenu.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));

	DeathScreen.SetButton(int(drestart),	L"\t     Начать Заново");
	DeathScreen.SetButton(int(dexit),		L"\t             Выйти");
	DeathScreen.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));

	pBackground.setBuffer(mWorld.mSounds.get(Sounds::ID::background));
	pBackground.setLoop(true);
	pBackground.setVolume(15);

	Menu_Sounds.resize(2);
	Menu_Sounds[Sounds_Menu::onButton].setBuffer(mWorld.mSounds.get(Sounds::ID::onButton));
	Menu_Sounds[Sounds_Menu::pressButton].setBuffer(mWorld.mSounds.get(Sounds::ID::pressButton));

	mPlayer.setSound(mWorld.mSounds);

	Button::SetTextureArk(mWorld.mTextures.get(Textures::button_ark));//устанавливаем тексуру для всех кнопок
	/*mWorld.plusView(sf::Vector2f(200, 200));*/
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			/*mPlayer.legs_state = Player::stop;*/
			timeSinceLastUpdate -= TimePerFrame;

			processInput();//проверка была ли нажата какая либо клавиша
			update(TimePerFrame);//проверка была ли нажата какая либо клавиша
			if (!IsInMainMenu && !IsInSubMenu && !mPlayer.IsInInventory() && mPlayer.GetHealth()>0)
			{
				mPlayer.Animate(TimePerFrame);//анимация игрока
				if (pBackground.getStatus() != sf::Sound::Playing)
					pBackground.play();
			}
			else if (mPlayer.IsInInventory() || IsInMainMenu || IsInSubMenu || mPlayer.GetHealth()>=0)
			if (pBackground.getStatus() == sf::Sound::Playing)
				pBackground.stop();
		}
	
		updateStatistics(elapsedTime);//этот метод показывает сколько в игре кадров в секунду
		render();//отрисовка всего, что было нарисовано в данной итерации
	}
}

void Game::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
		if (event.type == sf::Event::MouseMoved)
		{
			if (!IsInMainMenu && !IsInSubMenu && !mPlayer.IsInInventory() && mPlayer.GetHealth()>0)
				mPlayer.rotation(sf::Vector2i(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView())));
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape && !IsInMainMenu && !mPlayer.IsInInventory() && mPlayer.GetHealth()>0)
			{
				mWindow.setView(sf::View(sf::Vector2f(mWindow.getSize().x / 2, mWindow.getSize().y / 2), sf::Vector2f(mWindow.getSize())));
				IsInSubMenu = true;
			}
		}
		if (event.type == sf::Event::Resized)
		{
			mWorld.ChangeResolution(sf::FloatRect(0, 0, int(event.size.width), int(event.size.height)));
			mMenu.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));
			subMenu.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));
			DeathScreen.SetPositions(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));
		}
		if (IsInMainMenu)
			MainMenuInteractions(event);
		else if (IsInSubMenu)
			SubMenuInteractions(event);
		else if (mPlayer.GetHealth() <= 0)
			DeathScreenInteractions(event);
	}
}

void Game::MainMenuInteractions(sf::Event evnt)
{
	int a = mMenu.button_interactions(sf::Vector2i(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow))));
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))*/
		if (evnt.key.code == sf::Mouse::Left)
		{
			if (a == mstart)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				IsInMainMenu = false;
				mPlayer.setObject();
			}
			else if (a == mexit)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				mWindow.close();
			}
		}
	}
	else
	{
		static bool onButton = false;
		if (a >= mstart && mMenu.GetButtonsCount() > a)
		{
			if (!onButton)
			{
				if (Menu_Sounds[Sounds_Menu::onButton].getStatus() != sf::Sound::Playing)
					Menu_Sounds[Sounds_Menu::onButton].play();
				onButton = true;
			}
			mMenu[a].SetColor(sf::Color::Green);
		}
		else
		{
			for (int i = 0; i < mMenu.GetButtonsCount(); i++)
			{
				mMenu[i].SetColor(ORANGE);
			}
			onButton = false;
		}
	}
}

void Game::SubMenuInteractions(sf::Event evnt)
{
	int a = subMenu.button_interactions(sf::Vector2i(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow))));
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))*/
		if (evnt.key.code == sf::Mouse::Left)
		{
			if (a == mresume)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				IsInSubMenu = false;
			}
			else if (a == mtoMainMenu)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				IsInSubMenu = false;
				IsInMainMenu = true;
			}
			else if (a == mtoDesktop)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				mWindow.close();
			}
		}
	}
	else
	{
		static bool onButton = false;
		if (a >= mresume && subMenu.GetButtonsCount() > a)
		{
			if (!onButton)
			{
				if (Menu_Sounds[Sounds_Menu::onButton].getStatus() != sf::Sound::Playing)
					Menu_Sounds[Sounds_Menu::onButton].play();
				onButton = true;
			}
			subMenu[a].SetColor(sf::Color::Green);
		}
		else
		{
			for (int i = 0; i < subMenu.GetButtonsCount(); i++)
			{
				subMenu[i].SetColor(ORANGE);
			}
			onButton = false;
		}
	}
}

void Game::DeathScreenInteractions(sf::Event evnt)
{
	int a = DeathScreen.button_interactions(sf::Vector2i(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow))));
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))*/
		if (evnt.key.code == sf::Mouse::Left)
		{
			if (a == drestart)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				IsInMainMenu = false;
				//Enemies.erase(Enemies.begin(), Enemies.end());
			}
			else if (a == dexit)
			{
				Menu_Sounds[Sounds_Menu::pressButton].play();
				mWindow.close();
			}
		}
	}
	else
	{
		static bool onButton = false;
		if (a >= drestart && DeathScreen.GetButtonsCount() > a)
		{
			if (!onButton)
			{
				if (Menu_Sounds[Sounds_Menu::onButton].getStatus() != sf::Sound::Playing)
					Menu_Sounds[Sounds_Menu::onButton].play();
				onButton = true;
			}
			DeathScreen[a].SetColor(sf::Color::Green);
		}
		else
		{
			for (int i = 0; i < DeathScreen.GetButtonsCount(); i++)
			{
				DeathScreen[i].SetColor(ORANGE);
			}
			onButton = false;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	if (!IsInMainMenu && !IsInSubMenu && !mPlayer.IsInInventory() && mPlayer.GetHealth()>0)
	{
		mWindow.setMouseCursorVisible(false);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			mPlayer.update(mPlayer.Action::MoveUp, elapsedTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			mPlayer.update(mPlayer.Action::MoveDown, elapsedTime.asSeconds());
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			mPlayer.update(mPlayer.Action::MoveLeft, elapsedTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mPlayer.update(mPlayer.Action::MoveRight, elapsedTime.asSeconds());
		}

		mPlayer.second_update(elapsedTime, mWorld);

		mFight.FightDaPower(mWorld.mTextures.get(Textures::ammo));

		//<spawn enemies>
		int count_alive = 0;
		for (int i = 0; i < Enemies.size(); i++)//test_enemy
		{
			if (Enemies.at(i).GetHealth() > 0)
			{
				count_alive++;
			}
		}
		for (int i = 0; i < (MaxEnemies - count_alive); i++)//test_enemy
		{
			Enemies.push_back(Enemy(mWorld, mWindow, mPlayer, Enemies));//test_enemy
			Enemies[Enemies.size() - 1].SetTextures(mWorld.mTextures, Textures::ID::crab);//test_enemy
		}
		//</spawn enemies>

		for (int i = 0; i < Enemies.size(); i++)//test_enemy
		{
			if (Enemies[i].alive)
				Enemies[i].ToPlayer(elapsedTime);//test_enemy
		}

		mWorld.setView(sf::Vector2f(int(mPlayer.PlayerObject.GetPosition().x), int(mPlayer.PlayerObject.GetPosition().y)));
		mStatisticsText.setPosition(int(mPlayer.PlayerObject.GetPosition().x - mWindow.getSize().x / 2 + 5), int(mPlayer.PlayerObject.GetPosition().y - mWindow.getSize().y / 2 + 5));
	}
	else if (mPlayer.IsInInventory())
	{
		mWindow.setMouseCursorVisible(true);
		mPlayer.second_update(elapsedTime, mWorld);
		mWorld.setView(sf::Vector2f(int(mPlayer.PlayerObject.GetPosition().x), int(mPlayer.PlayerObject.GetPosition().y)));
	}
	else
	{
		mWindow.setMouseCursorVisible(true);
		mWorld.setView(sf::Vector2f(int(mWindow.getSize().x / 2), int(mWindow.getSize().y / 2)));
		ScoreInfo.setPosition(mWindow.getDefaultView().getCenter().x / 2, mWindow.getDefaultView().getCenter().y / 2 - 50);
	}
}

void Game::render()
{
	mWindow.clear();
	mWorld.ConfigureView();

	if (!IsInMainMenu && !IsInSubMenu && mPlayer.GetHealth()>0)
	{
		mWorld.draw();
		//mPlayer.cross.draw(mWindow);
		for (auto i:Enemies)//test_enemy
			i.Draw();//test_enemy
		mFight.Draw(mWindow);
		mPlayer.drawPlayer(mWindow);
		mWindow.draw(mStatisticsText);
	}
	else if (IsInMainMenu)
	{
		mMenu.DrawMenu(mWindow);
	}
	else if (IsInSubMenu)
	{
		subMenu.DrawMenu(mWindow);
	}
	else if (mPlayer.GetHealth() <= 0)
	{
		ScoreInfo.setString("Points: " + toString(mPlayer.Score));
		DeathScreen.DrawMenu(mWindow);
		mWindow.draw(ScoreInfo);
	}

	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;
	
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS:" + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}
