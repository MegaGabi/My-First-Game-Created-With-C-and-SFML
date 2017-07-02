#include "Button.h"
#include "Game.h"

sf::Sprite	Button::button_ark;

Button::Button()
{
	button_background.setPosition(100, 100);
}

Button::Button(sf::Vector2f pos, std::wstring str)
{
	button_background.setPosition(pos.x, pos.y);//местоположение кнопки на экране

	int biggest_x_str = 0;//подгон кнопки под текст
	for (int i = 0; i < str.size(); i++) 
	{
		if (i % 2 == 0)
		{
			biggest_x_str++;
		}
		if (str[i] == '\n')
		{
			if (biggest_x_str>button_size_added.x)
				button_size_added.x = biggest_x_str;
			biggest_x_str = 0;
			button_size_added.y++;
		}
		if (i == str.size() - 1)
		{
			if (biggest_x_str>button_size_added.x)
				button_size_added.x = biggest_x_str;
		}
	}
	button_background.setFillColor(ORANGE);
	button_background.setSize(sf::Vector2f((button_size_added.x * 32) - 2, ((button_size_added.y+1) * 32)+((button_size_added.y)?0:4)));

	//настройка текста
	mFont.loadFromFile("Media/Bricks.otf");
	mButtonText.setFont(mFont);
	mButtonText.setCharacterSize(26);
	mButtonText.setString(str);
	mButtonText.setColor(sf::Color::Black);
	mButtonText.setPosition(button_background.getPosition()+sf::Vector2f(8,4));
}

void Button::SetButton(sf::Vector2f pos, std::wstring str)
{
	button_background.setPosition(pos.x, pos.y);//местоположение кнопки на экране

	int biggest_x_str = 0;//подгон кнопки под текст
	for (int i = 0; i < str.size(); i++)
	{
		if (i % 2 == 0)
		{
			biggest_x_str++;
		}
		if (str[i] == '\n')
		{
			if (biggest_x_str>button_size_added.x)
				button_size_added.x = biggest_x_str;
			biggest_x_str = 0;
			button_size_added.y++;
		}
		if (i == str.size() - 1)
		{
			if (biggest_x_str>button_size_added.x)
				button_size_added.x = biggest_x_str;
		}
	}
	button_background.setFillColor(ORANGE);
	button_background.setSize(sf::Vector2f((button_size_added.x * 32) - 2, ((button_size_added.y + 1) * 32) + ((button_size_added.y) ? 0 : 4)));

	//настройка текста
	mFont.loadFromFile("Media/Bricks.otf");
	mButtonText.setFont(mFont);
	mButtonText.setCharacterSize(26);
	mButtonText.setString(str);
	mButtonText.setColor(sf::Color::Black);
	mButtonText.setPosition(button_background.getPosition() + sf::Vector2f(8, 4));
}

bool Button::OnButton(sf::Vector2i mpos)//проверка наход€тс€ ли данные координаты на кнопке
{
	if (button_background.getGlobalBounds().contains(sf::Vector2f(mpos.x, mpos.y)))
		return true;
	return false;
}

void Button::SetColor(sf::Color col)//установка цвета кнопки
{
	button_background.setFillColor(col);
}

void Button::SetPosition(sf::Vector2f pos)
{
	button_background.setPosition(pos);
	mButtonText.setPosition(button_background.getPosition() + sf::Vector2f(8, 4));
}

void Button::SetTextureArk(sf::Texture &ptxt)//установка текстуры рамки кнопки
{
	button_ark.setTexture(ptxt);
}

const sf::FloatRect Button::GetRect()
{
	return(button_background.getGlobalBounds());
}

const std::wstring Button::GetButtonName()
{
	return mButtonText.getString();
}

void Button::DrawButton(sf::RenderWindow& mWind)//рисует кнопку
{
	//рисует текст и фон кнопки
	mWind.draw(button_background);
	mWind.draw(mButtonText);

	//рамка кнопки
	button_ark.setPosition(button_background.getPosition());
	for (int i = 0; i < button_size_added.y+1; i++)
	{
		for (int j = 0; j < button_size_added.x; j++)
		{
			bool draw = true;//переменна€ указывающа€ на то нужно ли рисовать объект на данной итерации
			//верхн€€ часть кнопки
			if (!i)
			{
				if (!j)
					button_ark.setTextureRect(sf::IntRect(0, 0, 32, 32));
				else if (j == button_size_added.x - 1)
					button_ark.setTextureRect(sf::IntRect(64, 0, 32, 32));
				else
					button_ark.setTextureRect(sf::IntRect(32, 0, 32, 32));
			}
			//нижн€€ часть кнопки
			else if (i == button_size_added.y)
			{
				if (!j)
					button_ark.setTextureRect(sf::IntRect(0, 64, 32, 32));
				else if (j == button_size_added.x - 1)
					button_ark.setTextureRect(sf::IntRect(64, 64, 32, 32));
				else
					button_ark.setTextureRect(sf::IntRect(32, 64, 32, 32));
			}
			//середина кнопки
			else
			{
				if (!j)
					button_ark.setTextureRect(sf::IntRect(0, 32, 32, 32));
				else if (j == button_size_added.x - 1)
					button_ark.setTextureRect(sf::IntRect(64, 32, 32, 32));
				else
					draw = false;
			}
			//рамка рисуетс€ здесь
			if (draw)
				mWind.draw(button_ark);
			//двигаем спрайт в другое место дл€ отрисовки по оси х
			button_ark.move(32, 0);
		}
		//двигаем спрайт вниз по оси у и в начало оси х(относительно кнопки)
		button_ark.move(-(button_size_added.x * 32), 32);
	}

	//если в кнопке только строка текста, то этот код нарисует нижнию часть рамки
	if (!button_size_added.y)
	{
		for (int j = 0; j < button_size_added.x; j++)
		{
			if (!j)
				button_ark.setTextureRect(sf::IntRect(0, 90, 32, 6));
			else if (j == button_size_added.x - 1)
				button_ark.setTextureRect(sf::IntRect(64, 90, 32, 6));
			else
				button_ark.setTextureRect(sf::IntRect(32, 90, 32, 6));
			mWind.draw(button_ark);
			button_ark.move(32, 0);
		}
	}

}
