#include "stdafx.h"
#include "AnimationController.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

AnimationController::AnimationController()
	: m_spriteRender{ NULL }
{}

AnimationController::~AnimationController() { m_spriteRender = NULL; }

void AnimationController::AddAnimation(Animation& animation)
{
	auto itr = m_animations.find(animation.name);
	if (itr != m_animations.end()) { return; }

	m_animations.emplace(animation.name, animation);
}

void AnimationController::AddAnimationRange(std::vector<Animation>&& animations)
{
	for (auto& animationIt : animations)
	{
		auto itr = m_animations.find(animationIt.name);
		if (itr != m_animations.end()) { continue; }

		m_animations.emplace(animationIt.name, animationIt);
	}
}

void AnimationController::SetAnimation(std::string name)
{
	if (m_animations.find(name) == m_animations.end()) { return; }

	m_currentAnimation = name;
	Animation animation = m_animations.find(name)->second;

	Rect destinationRect;
	Transform transform = owner()->transform;

	m_frameBounds.left = animation.startFrame * (float)animation.frameWidth;
	m_frameBounds.top = animation.spriteSheetRow * (float)animation.frameHeight;
	m_frameBounds.right = m_frameBounds.left + (float)animation.frameWidth;
	m_frameBounds.bottom = m_frameBounds.top + (float)animation.frameHeight;
		
	m_spriteRender->sourceRect() = m_frameBounds;

	Animation* anim = &m_animations.find(name)->second;

	anim->currentFrame = anim->startFrame;
	anim->elapsed = 0.0;
	anim->currentFrame += anim->frameStep;
}

void AnimationController::NextFrame()
{
	if (m_animations.empty()) { return; }
	if (m_currentAnimation == "") { return; }

	Animation* animation = &m_animations.find(m_currentAnimation)->second;

	bool finished = animation->frameStep > 0 ? animation->currentFrame > animation->endFrame ? true : false : animation->currentFrame < animation->endFrame ? true : false;

	if (finished)
	{
		animation->currentFrame = animation->startFrame;

		if (animation->repeat) 
		{ 
			SetAnimation(m_currentAnimation);
			return; 
		}
		if (animation->transition != nullptr)
		{
			SetAnimation(animation->transition->name);
			//m_currentAnimation = animation->transition->name;
			return;
		}

		m_currentAnimation = "";
		return;
	}
	
	Rect destinationRect;
	Transform transform = owner()->transform;

	m_frameBounds.left = animation->currentFrame * (float)animation->frameWidth;
	m_frameBounds.top = animation->spriteSheetRow * (float)animation->frameHeight;
	m_frameBounds.right = m_frameBounds.left + animation->frameWidth;
	m_frameBounds.bottom = m_frameBounds.top + animation->frameHeight;

	m_spriteRender->sourceRect() = m_frameBounds;
	animation->currentFrame += animation->frameStep;
}

void AnimationController::spriteRenderer(SpriteRenderer* spriteRenderer)
{
	m_spriteRender = spriteRenderer;
}

Animation* AnimationController::currentAniamtion()
{
	return  m_currentAnimation != "" ? &m_animations[m_currentAnimation] : nullptr;
}

void AnimationController::SetTransition(std::string animationName, std::string transitionName)
{
	if (m_animations.find(transitionName) == m_animations.end()) { return; }
	if (m_animations.find(animationName) == m_animations.end()) { return; }

	m_animations.find(animationName)->second.transition = &m_animations.find(transitionName)->second;
}