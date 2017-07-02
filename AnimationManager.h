#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class AnimationManager
{
private:
	sf::Sprite			animated_sprite;

	float				CurrFrame;

	int					TotalFrames,
						StartFrame,
						StopFrame;

	float					Speed;

	sf::Vector2f		size;

public:
	enum				enum_state{start, stop}state;

public:
						AnimationManager(int TotalFrames);
						AnimationManager(int TotalFrames, sf::Vector2f size);
						AnimationManager(int TotalFrames, int StartFrame, int StopFrame, sf::Vector2f size);
	void				Animation(const sf::Time& s_time);
	void				SetSpeed(int s);
	void				SetState(enum_state st);
	void				SetTotalFrames(int total);
	void				SetStartFrame(int start);
	void				SetStopFrame(int stop);
	sf::Sprite&			GetSprite();
	enum_state			GetState();
	float&				GetFrame();
};

