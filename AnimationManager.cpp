#include "AnimationManager.h"

AnimationManager::AnimationManager(int TotalFrames) :StopFrame(0), StartFrame(1)
{
	this->TotalFrames = TotalFrames;
	CurrFrame = StopFrame;
	Speed = 20;
	size = sf::Vector2f(32, 32);
}

AnimationManager::AnimationManager(int TotalFrames, sf::Vector2f size) :StopFrame(0), StartFrame(1)
{
	this->TotalFrames = TotalFrames;
	CurrFrame = StopFrame;
	Speed = 20;
	this->size = size;
}

AnimationManager::AnimationManager(int TotalFrames, int StartFrame, int StopFrame, sf::Vector2f size)
{
	this->TotalFrames = TotalFrames;
	this->StartFrame = StartFrame;
	this->StopFrame = StopFrame;
	CurrFrame = StartFrame;
	Speed = 20;
	this->size = size;
}

void AnimationManager::Animation(const sf::Time& s_time)
{
	if (state == start)
	{
		CurrFrame += s_time.asSeconds() * Speed;
		if (CurrFrame >= TotalFrames || CurrFrame < 0) /*CurrFrame -= TotalFrames;*/Speed *= -1;
	}
	else
	{
		CurrFrame = StopFrame;
	}
	animated_sprite.setTextureRect(sf::IntRect(int(CurrFrame) * size.x, 0, size.x, size.y));
}

void AnimationManager::SetSpeed(int s)
{
	Speed = s;
}

void AnimationManager::SetState(enum_state st)
{
	state = st;
}

void AnimationManager::SetTotalFrames(int total)
{
	TotalFrames = total;
}

void AnimationManager::SetStartFrame(int start)
{
	StartFrame = start;
}

void AnimationManager::SetStopFrame(int stop)
{
	StopFrame = stop;
}

sf::Sprite& AnimationManager::GetSprite()
{
	return animated_sprite;
}

AnimationManager::enum_state AnimationManager::GetState()
{
	return state;
}

float& AnimationManager::GetFrame()
{
	return CurrFrame;
}