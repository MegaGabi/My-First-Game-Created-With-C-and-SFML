#include "inventory.h"
#include "Game.h"

inventory::inventory() :inventory_animation(2, 0, 2, sf::Vector2f(470, 82)), isShown(false), isAnimation(false), current_menu(weapon)
{}

void inventory::start()
{
	inventory_animation.SetState(AnimationManager::enum_state::start);
	isAnimation = true;
	isShown = false;
}

void inventory::stop()
{
	inventory_animation.SetState(AnimationManager::enum_state::start);
	if (!isAnimation)
	{
		isShown = true;
		isAnimation = true;
		inventory_animation.GetFrame() = 2;
	}
}

void inventory::setTextures(TextureHolder& pTxtrs)
{
	inventory_animation.GetSprite().setTexture(pTxtrs.get(Textures::inventory_animation));
	inventory_animation.SetSpeed(8);

	inventory_face.setTexture(pTxtrs.get(Textures::inventory_face));
	armour_info.setTexture(pTxtrs.get(Textures::inventory_armour_info));
	list_grenade_and_shield.setTexture(pTxtrs.get(Textures::inventory_list_grenade));
	list_weapon.setTexture(pTxtrs.get(Textures::inventory_list_weapons));
	marker.setTexture(pTxtrs.get(Textures::inventory_marker));
	icon.setTexture(pTxtrs.get(Textures::inventory_icons));
	//test
	icon.setColor(sf::Color(64, 64, 64));
	choosed_icon.setTexture(pTxtrs.get(Textures::inventory_icons));
	choosed_icon.setTextureRect(
		sf::IntRect(gun_s_icon.left - posToDrawIcons.x, gun_s_icon.top - posToDrawIcons.y, gun_s_icon.width, gun_s_icon.height));
	choosed_icon.setColor(ORANGE);
	//\test
	weapon_slots.setTexture(pTxtrs.get(Textures::inventory_weapon_slots));

	inventory_animation.SetState(AnimationManager::enum_state::start);
}

bool& inventory::isInventory()
{
	if (isShown)
		return isShown;
	else if (isAnimation)
	{
		return isAnimation;
	}
	else
		return isShown;
}

void inventory::animate(sf::Time t)
{
	if (!isShown)
	{
		if (int(inventory_animation.GetFrame()) == 2)
		{
			inventory_animation.SetState(AnimationManager::enum_state::stop);
			inventory_animation.GetFrame() = 0;
			isAnimation = false;
			isShown = true;
		}
		else
			inventory_animation.Animation(t);
	}
	else if (isAnimation)
	{
		if (int(inventory_animation.GetFrame()) == 0)
		{
			inventory_animation.SetState(AnimationManager::enum_state::stop);
			isAnimation = false;
			isShown = false;
		}
		else
			inventory_animation.Animation(t);
	}
}

void inventory::update(sf::Vector2f pos, sf::RenderWindow& W)
{
	sf::Vector2f mouse_pos;
	/*printf("%f : %f\n", pos.x, pos.y);*/
	inventory_face.setPosition(pos - sf::Vector2f(inventory_face.getGlobalBounds().width / 2, inventory_face.getGlobalBounds().height));

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isShown)
	{
		/*mouse_pos = sf::Vector2f(sf::Mouse::getPosition());*/
		
		mouse_pos = W.mapPixelToCoords(sf::Mouse::getPosition(W), W.getView());
		sf::FloatRect add_to_pos = sf::FloatRect(inventory_face.getPosition(), sf::Vector2f(0.f, 0.f));
		if (
			(sf::FloatRect(
			gun_s_icon.left + add_to_pos.left,
			gun_s_icon.top + add_to_pos.top,
			gun_s_icon.width,
			gun_s_icon.height)
			).contains(sf::Vector2f(mouse_pos))
		   )
		{
			current_menu = weapon;
			choosed_icon.setTextureRect(
				sf::IntRect(gun_s_icon.left - posToDrawIcons.x, gun_s_icon.top - posToDrawIcons.y, gun_s_icon.width, gun_s_icon.height));
			printf("guns_here\n");
		}
		else if (
			(sf::FloatRect(
			grenade_icon.left + add_to_pos.left,
			grenade_icon.top + add_to_pos.top,
			grenade_icon.width,
			grenade_icon.height)
			).contains(sf::Vector2f(mouse_pos))
			)
		{
			current_menu = grenade;
			marker.setPosition(posFirstMarker + inventory_face.getPosition());
			choosed_icon.setTextureRect(
				sf::IntRect(grenade_icon.left - posToDrawIcons.x, grenade_icon.top - posToDrawIcons.y, grenade_icon.width, grenade_icon.height));
			printf("grenade_here\n");
		}
		else if (
			(sf::FloatRect(
			shield_icon.left + add_to_pos.left,
			shield_icon.top + add_to_pos.top,
			shield_icon.width,
			shield_icon.height)
			).contains(sf::Vector2f(mouse_pos))
			)
		{
			current_menu = shield;
			marker.setPosition(posSecondMarker + inventory_face.getPosition());
			choosed_icon.setTextureRect(
				sf::IntRect(shield_icon.left - posToDrawIcons.x, shield_icon.top - posToDrawIcons.y, shield_icon.width, shield_icon.height));
			printf("shield_here\n");
		}
		else if (
			(sf::FloatRect(
			armor_icon.left + add_to_pos.left,
			armor_icon.top + add_to_pos.top,
			armor_icon.width,
			armor_icon.height)
			).contains(sf::Vector2f(mouse_pos))
			)
		{
			current_menu = armour;
			choosed_icon.setTextureRect(
				sf::IntRect(armor_icon.left - posToDrawIcons.x, armor_icon.top - posToDrawIcons.y, armor_icon.width, armor_icon.height));
			printf("armour_here\n");
		}
	}



	inventory_animation.GetSprite().setPosition
		(pos - sf::Vector2f(inventory_animation.GetSprite().getGlobalBounds().width / 2,
		inventory_animation.GetSprite().getGlobalBounds().height));
	
	icon.setPosition(inventory_face.getPosition() + posToDrawIcons);

	list_grenade_and_shield.setPosition(inventory_face.getPosition() + posToDrawGrenadeList);
	list_weapon.setPosition(inventory_face.getPosition() + posToDrawWeaponList);
	armour_info.setPosition(inventory_face.getPosition() + posToDrawArmorInfo);
	weapon_slots.setPosition(inventory_face.getPosition() + posToDrawWeaponSlot);

	switch (current_menu)
	{
	case weapon:
		choosed_icon.setPosition(icon.getPosition());
		break;
	case shield:
		choosed_icon.setPosition(inventory_face.getPosition() + sf::Vector2f(shield_icon.left, shield_icon.top));
		marker.setPosition(posFirstMarker + inventory_face.getPosition());
		break;
	case grenade:
		choosed_icon.setPosition(inventory_face.getPosition() + sf::Vector2f(grenade_icon.left, grenade_icon.top));
		marker.setPosition(posSecondMarker + inventory_face.getPosition());
		break;
	case armour:
		choosed_icon.setPosition(inventory_face.getPosition() + sf::Vector2f(armor_icon.left, armor_icon.top));
		break;
	}
}

void inventory::Draw(sf::Vector2f pos, sf::RenderWindow& mWind)
{
	update(pos, mWind);

	if (isAnimation)
		mWind.draw(inventory_animation.GetSprite());
	else if (isShown)
	{
		mWind.draw(choosed_icon);
		mWind.draw(inventory_face);
		
		switch (current_menu)
		{
		case grenade:
		case shield:
			mWind.draw(list_grenade_and_shield); mWind.draw(weapon_slots); mWind.draw(marker); break;
		case weapon:mWind.draw(list_weapon); mWind.draw(weapon_slots);	 break;
		case armour:mWind.draw(armour_info);	 break;
		}
		
		mWind.draw(icon);
		mWind.draw(choosed_icon);
	}
}
