#ifndef BOOK_RESOURCEIDENTIFIERS_HPP
#define BOOK_RESOURCEIDENTIFIERS_HPP


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		crosshair,

		Player_body,
		Player_legs,

		//<enemies>
		Dummy,
		crab,
		//</enemies>

		//<weapon's - src>
		arms,
		bullet,
		ammo,
		//</weapon's - src>

		button_ark,

		//<hood - src>
		hood_texture,
		hood_texture_fill,
		//</hood - src>

		//<inventory - src>
		inventory_animation,
		inventory_face,
		inventory_list_weapons,
		inventory_list_grenade,
		inventory_armour_info,
		inventory_marker,
		inventory_icons,
		inventory_weapon_slots
		//</inventory - src>
	};
}

namespace Sounds
{
	enum ID
	{
		shot,
		cock,
		reload,
		step, 
		background,
		dying_monster,
		onButton,
		pressButton
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundHolder;

#endif // BOOK_RESOURCEIDENTIFIERS_HPP
