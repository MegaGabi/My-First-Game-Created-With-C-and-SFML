#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>



class Button
{	
	sf::RectangleShape				button_background;
	sf::Font						mFont;
	sf::Text						mButtonText;
	static sf::Sprite				button_ark;
	sf::Vector2f					button_size_added;
public:
	Button();
	Button(sf::Vector2f pos, std::wstring str);
	void SetButton(sf::Vector2f pos, std::wstring str);
	bool OnButton(sf::Vector2i mpos);
	void SetColor(sf::Color col);
	void SetPosition(sf::Vector2f pos);
	static void SetTextureArk(sf::Texture &ptxt);
	const sf::FloatRect GetRect();
	const std::wstring GetButtonName();
	void DrawButton(sf::RenderWindow& mWind);
};

