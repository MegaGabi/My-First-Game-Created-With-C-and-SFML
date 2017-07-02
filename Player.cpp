#include "Player.h"
#include "helper/StringHelpers.hpp"

//функции для "облегчения чтения"
void Normalize_Legs(AnimationManager &animation_legs, Player::Action button, Player::Action previous_action)
{
	
	if (button != previous_action)
	{		
		//ф-ция работает не корректно если градус равен нулю когда он должен быть равен 360
		button			= Player::Action(button          == 0 && previous_action != 90 ? 360 :          button);
		previous_action = Player::Action(previous_action == 0 && button          != 90 ? 360 : previous_action);
		//

		if (button < previous_action)
			animation_legs.GetSprite().setRotation(button + 45);
		else
			animation_legs.GetSprite().setRotation(previous_action + 45);
	}
	else
		animation_legs.GetSprite().setRotation(button);
}

Player::Player(tmx::MapLoader& plvl, sf::RenderWindow& W, sf::Font& mFont)
:shoot_time(sf::seconds(0)), speed(150), Entity(plvl), animation_legs(10), W(W), Score(0)
{
	Gun.insert(std::pair<weapon_class::Gun_type, weapon_class>(weapon_class::rifle, weapon_class(sprite_body)));
	CurrGun = weapon_class::rifle;
	armor = 100;
	live  = 100;

	mStatisticsText.setFont(mFont);
	mStatisticsText.setCharacterSize(10);
}

void Player::setTexture(TextureHolder& ptexture)//устанавливает текстуры игрока из специального контейнера текстур
{
	cross.SetTexture(ptexture.get(Textures::ID::crosshair));

	sprite_body.setTexture(ptexture.get(Textures::Player_body));
	sprite_body.setOrigin(sprite_body.getGlobalBounds().width / 2, sprite_body.getGlobalBounds().height / 2);
		//определяет центр спрайта, это нужно для вращения
	animation_legs.GetSprite().setTexture(ptexture.get(Textures::Player_legs));
	animation_legs.GetSprite().setTextureRect(sf::IntRect(0, 0, 32, 32));
	animation_legs.GetSprite().setOrigin
		(animation_legs.GetSprite().getGlobalBounds().width / 2, animation_legs.GetSprite().getGlobalBounds().height / 2);
			//определяет центр спрайта, это нужно для вращения
	/*legs_state = stop;*/
	animation_legs.SetState(AnimationManager::stop);//устанавливает статус анимации ног

	Gun.at(CurrGun).setCurWeapon(ptexture.get(Textures::arms), CurrGun);

	mHood.SetTexture(ptexture);//TEST!!!ACHTUNG!!! 
	mInventory.setTextures(ptexture);//TEST!!!ACHTUNG!!! 
}

/*
"Вытягивает" объект игрока из .tmx файла
и придает все его значения спрайтам игрока
*/
void Player::setObject()
{
	for (auto layer = mlvl.GetLayers().begin(); layer != mlvl.GetLayers().end(); ++layer)
	{

		if (layer->name == "objects")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
			{
				if (object->GetName() == "Player")
				{
					PlayerObject = (*object);
					x = PlayerObject.GetPosition().x;
					y = PlayerObject.GetPosition().y;
				}
			}
		}
	}
	sprite_body.setPosition(PlayerObject.GetCentre());
	animation_legs.GetSprite().setPosition(sprite_body.getPosition());
	if (Gun.at(CurrGun).IsArmed())
		Gun.at(CurrGun).SetPosition(sprite_body.getPosition());
}

void Player::setSound(SoundHolder& snd)
{
	Gun.at(CurrGun).setSound(snd);

	plrSounds.resize(2);
	plrSounds[snd_step].setBuffer(snd.get(Sounds::ID::step));
}

sf::Sprite Player::GetBodySprite()
{
	return sprite_body;
}

//Вращает игрока в сторону курсора
void Player::rotation(sf::Vector2i pM_pos)
{
	float rotation = (atan2(PlayerObject.GetCentre().y - pM_pos.y, PlayerObject.GetCentre().x - pM_pos.x)) * 180 / 3.14159265;

	if (Gun.at(CurrGun).IsArmed())
	{
		sprite_body.setRotation(rotation - 70);
		Gun.at(CurrGun).rotate(rotation - 90);
	}
	else
		sprite_body.setRotation(rotation - 90);
}

//все взаимодействие игрока с миром
void Player::update(Action button, float time)
{
	if (plrSounds.at(snd_step).getStatus() == sf::Sound::Stopped)
		plrSounds.at(CurrGun).play();

	static Action previous_action;
	switch (button)
	{
		case MoveRight:	dx = speed;	  break;
		case MoveLeft:	dx = -speed;  break;
		case MoveUp:	dy = -speed;  break;
		case MoveDown:	dy = speed;	  break;
	}

	Normalize_Legs(animation_legs, button, previous_action);
	animation_legs.SetState(AnimationManager::start);

	previous_action = button;

	x += dx*time;
	StandartMapCollisison(dx, 0, x, y, sprite_body.getLocalBounds());
	y += dy*time;
	StandartMapCollisison(0, dy, x, y, sprite_body.getLocalBounds());

	dx = 0;
	dy = 0;

	PlayerObject.SetPosition(int(x), int(y));

	//определение всех частей персонажа в пространстве
	sprite_body.setPosition(PlayerObject.GetCentre());
	animation_legs.GetSprite().setPosition(sprite_body.getPosition());
	Gun.at(CurrGun).SetPosition(sprite_body.getPosition());
}

void Player::second_update(sf::Time t, World& mWorld)
{

	static bool TABpressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		if (!TABpressed)
		{
			/*if (!mInventory.isInventory())
				mInventory.start();
			else
				mInventory.stop();*/
			TABpressed = true;
			mInventory.isInventory() = !mInventory.isInventory();
		}
	}
	else
		TABpressed = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !Gun.at(CurrGun).GetShooted() && !IsInInventory())
		Gun.at(CurrGun).shot(mWorld.mlvl, mWorld.mTextures.get(Textures::bullet), GetPlayerPosititon());

	Gun.at(CurrGun).update_bullet(TimePerFrame);
}

//рисует все что связано с данным классом
void Player::drawPlayer(sf::RenderWindow& W)
{
	mHood.update(sf::Vector2f(W.getSize()), this);
	if (!IsInInventory())
	{
		cross.update(W);
	}

	animation_legs.SetState(AnimationManager::stop);
	W.draw(animation_legs.GetSprite());
	Gun.at(CurrGun).draw(W);
	W.draw(sprite_body);

	if (!mInventory.isInventory()) // invent
	{
		cross.draw(W);
		mHood.DrawHood(W); //TEST!!!ACHTUNG!!! 

		//вывести инфу про пули
		mStatisticsText.setString(toString(Gun.at(CurrGun).GetBulletsInfo().first) + "\n" + toString(Gun.at(CurrGun).GetBulletsInfo().second));
		mStatisticsText.setPosition(cross.GetPosition() + sf::Vector2f(5,5));

		W.draw(mStatisticsText);
	}
	//else
	//	mInventory.Draw(mHood.GetPosition(), W); // invent
}

bool Player::IsInInventory()
{
	return mInventory.isInventory();
}

//возвращает местоположение игрока
sf::Vector2f Player::GetPlayerPosititon()
{
	return sprite_body.getPosition();
}

sf::FloatRect Player::GetPlayerRect()
{
	return sprite_body.getGlobalBounds();
}

int& Player::GetArmor()
{
	return armor;
}

int& Player::GetHealth()
{
	return live;
}

//анимация
void Player::Animate(const sf::Time& time)
{
	Gun.at(CurrGun).ArmsAnimation(time);
	animation_legs.Animation(time);
	//if (mInventory.isInventory())
	//	mInventory.animate(time);//ACHTUNG!!!
}