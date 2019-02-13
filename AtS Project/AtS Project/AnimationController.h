#pragma once
#include <string>
#include <map>
#include <vector>
#include "Component.h"
#include "Math.h"

struct Animation
{
	Animation(std::string l_name, int l_startFrame, int l_endFrame, double l_speed, UINT l_spriteSheetRow, UINT l_frameWidth, UINT l_frameHeight, bool l_repeat = false)
		: name{ l_name }, startFrame{ l_startFrame }, endFrame{l_endFrame}, frameTime{ l_speed },
		spriteSheetRow{ l_spriteSheetRow }, frameWidth{ l_frameWidth }, frameHeight{ l_frameHeight }, repeat{ l_repeat }
	{
		currentFrame = startFrame;
		frameNumber = endFrame + 1;
		frameStep = 1;
		elapsed = 0.0;
		transition = nullptr;
	}

	Animation() {}

	std::string name;
	int currentFrame;
	int frameNumber;
	int startFrame;
	int endFrame;
	int frameStep;
	double frameTime;
	double elapsed;
	UINT frameWidth;
	UINT frameHeight;
	UINT spriteSheetRow;
	bool repeat;
	Animation* transition;
};


class SpriteRenderer;

class AnimationController : public Component
{
public:
	AnimationController();
	~AnimationController();

	void spriteRenderer(SpriteRenderer* spriteRenderer);

	void AddAnimation(Animation& animation);
	void AddAnimationRange(std::vector<Animation>&& animations);

	Animation* currentAniamtion();
	void SetAnimation(std::string name);

	void NextFrame();

	void SetTransition(std::string animationName, std::string transitionName);

private:
	SpriteRenderer* m_spriteRender;
	
	std::map<std::string, Animation> m_animations;

	std::string m_currentAnimation;
	Rect m_frameBounds;

};