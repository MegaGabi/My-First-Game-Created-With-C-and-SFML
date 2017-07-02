#pragma once
#include "Button.h"

class main_menu
{
	std::vector<Button*> buttons;
	sf::Vector2f first_pos;
	int all_buttons;
public:
	main_menu();
	main_menu(sf::Vector2f first_pos);
	void SetButton(int index, const std::wstring& str);
	void SetPositions(sf::Vector2f first_pos);
	int button_interactions(sf::Vector2i m_pos);
	Button& operator[](int index);
	int GetButtonsCount();
	void DrawMenu(sf::RenderWindow& mWind);
};

