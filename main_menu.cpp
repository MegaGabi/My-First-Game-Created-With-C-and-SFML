#include "main_menu.h"

main_menu::main_menu()
{
	first_pos = sf::Vector2f(200, 200);
}

main_menu::main_menu(sf::Vector2f first_pos)
{
	this->first_pos = first_pos;
}

void main_menu::SetButton(int index, const std::wstring& str)
{
	sf::Vector2f temp = first_pos;
	for (int i = 0; i < index; i++)
	{
		temp += sf::Vector2f(0, buttons[i]->GetRect().height + 20);
	}
	buttons.push_back(new Button(temp, str));
	all_buttons++;
}

void main_menu::SetPositions(sf::Vector2f first_pos)
{
	sf::Vector2f temp = sf::Vector2f(buttons[0]->GetRect().width, 0);
	for (int i = 0; i < all_buttons; i++)
	{
		temp += sf::Vector2f(0, buttons[i]->GetRect().height + 20);
	}
	temp = first_pos - sf::Vector2f(temp.x / 2, (temp.y - 20));
	/*std::cout << "Menu center: " << temp.x << "x" << temp.y << endl;*/

	for (int i = 0; i < all_buttons; i++)
	{
		temp += sf::Vector2f(0, buttons[i]->GetRect().height + 20);
		buttons[i]->SetPosition(sf::Vector2f(int(temp.x), int(temp.y)));
	}
}

int main_menu::button_interactions(sf::Vector2i m_pos)
{
	for (int i = 0; i < all_buttons; i++)
	{
		if (buttons[i]->OnButton(m_pos))
			return i;
	}
	return all_buttons;
}

Button& main_menu::operator[](int index)
{
	return (*buttons[index]);
}

int main_menu::GetButtonsCount()
{
	return all_buttons;
}

void main_menu::DrawMenu(sf::RenderWindow& mWind)
{
	for (int i = 0; i < all_buttons; i++)
		buttons[i]->DrawButton(mWind);
}
