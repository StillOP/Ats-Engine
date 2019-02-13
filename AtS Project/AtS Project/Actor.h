#pragma once
#include "GameObjectsCollection.h"
#include "GameObject.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "Direction.h"

class Actor: public Behaviour
{
public:
	Actor(Haviour haviour);
	~Actor();

	void Init();
	void Update();
	
	void OnStateEntered(unsigned int state);
	void OnStateUpdate(unsigned int state);
	void OnStateExit(unsigned int state);

private:
	SpriteRenderer* m_spriteRenderer;
	AnimationController* m_animationController;
	StateController* m_stateController;
	
	enum class ActorState
	{
		IDLE = 0, RUNNING, CHARGING, CHARGING_WHILE_RUNNING, FIRE
	};
};