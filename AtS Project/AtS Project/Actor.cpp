#include "stdafx.h"
#include "Actor.h"
#include "Time.h"

Actor::Actor(Haviour haviour)
	: Behaviour(haviour)
{}

Actor::~Actor() {}

void Actor::Init()
{
	m_spriteRenderer = m_owner->AddComponent<SpriteRenderer>();
	m_animationController = m_owner->AddComponent<AnimationController>();
	m_stateController = m_owner->AddComponent<StateController>();

	m_spriteRenderer->bitmap(ResourceSystem::get().GetBitmap(
		"", 
		"C:\\Users\\Pierre\\source\\repos\\AtS Project\\Assets\\ActorSheet.png"),
		Rect(0.0f, 0.0f, 1000.0f, 1000.0f)
	);
	m_animationController->spriteRenderer(m_spriteRenderer);

	m_animationController->AddAnimationRange({
		Animation("idleR", 0, 5, 0.17, 0, 1000, 1000, true),
		Animation("idleL", 0, 5, 0.17, 1, 1000, 1000, true),
		Animation("runR", 0, 6, 0.125, 2, 1000, 1000, true),
		Animation("runL", 0, 6, 0.125, 3, 1000, 1000),
		Animation("fireR", 1, 2, 0.17, 4, 1000, 1000),
		Animation("chargeR", 0, 0, 0.17, 4, 1000, 1000),
		Animation("fireL", 0, 2, 0.17, 5, 1000, 1000),
		Animation("cwrR", 0, 6, 0.125, 6, 1000, 1000, true),
		Animation("cwrL", 0, 6, 0.125, 7, 1000, 1000)
		});
	
	/*m_animationController->SetAnimation("idleR");
	m_animationController->SetTransition("fireL", "idleL");
	m_animationController->SetTransition("fireR", "idleR");*/

	m_stateController->AddStateRange({
		(unsigned int)ActorState::IDLE,
		(unsigned int)ActorState::RUNNING,
		(unsigned int)ActorState::FIRE,
		(unsigned int)ActorState::CHARGING,
		(unsigned int)ActorState::CHARGING_WHILE_RUNNING
		});
	
	m_stateController->SetState((unsigned int)ActorState::IDLE);
}

void Actor::Update()
{
	InputSystem* inputSys = &InputSystem::get();
	double deltaTime = Time::get().deltaTime;

	if (inputSys->IsKeyPressed(0x25))
	{
		m_owner->transform.Translate(Vector2(800.0f * deltaTime, 0.0f));
	}
}

void Actor::OnStateEntered(unsigned int state) 
{
	switch (state)
	{
		case (unsigned int)ActorState::IDLE:
		{
			m_animationController->SetAnimation("idleR");
			break;
		}
		case (unsigned int)ActorState::RUNNING:
		{
			m_animationController->SetAnimation("runR");
			break;
		}
		case (unsigned int)ActorState::CHARGING:
		{
			m_animationController->SetAnimation("chargeR");
			break;
		}
		case (unsigned int)ActorState::CHARGING_WHILE_RUNNING:
		{
			m_animationController->SetAnimation("cwrR");
			break;
		}
		case (unsigned int)ActorState::FIRE:
		{
			m_animationController->SetAnimation("fireR");
			break;
		}
	}
}

void  Actor::OnStateUpdate(unsigned int state)
{
	InputSystem* inputSys = &InputSystem::get();
	double deltaTime = Time::get().deltaTime;

	switch (state)
	{
	case (unsigned int)ActorState::IDLE:
	{
		if (inputSys->IsKeyPressed(0x27))
		{
			m_stateController->SetState((unsigned int)ActorState::RUNNING);
			break;
		}
		else if (inputSys->IsKeyPressed(0x41))
		{
			m_stateController->SetState((unsigned int)ActorState::CHARGING);
			break;
		}

		break;
	}
	case (unsigned int)ActorState::RUNNING:
	{
		if (inputSys->IsKeyJustReleased(0x27))
		{
			m_stateController->SetState((unsigned int)ActorState::IDLE);
			break;
		}
		else if (inputSys->IsKeyPressed(0x41))
		{
			m_stateController->SetState((unsigned int)ActorState::CHARGING);
			break;
		}
		
		m_owner->transform.Translate(Vector2(800.0f * deltaTime, 0.0f));

		break;
	}
	case (unsigned int)ActorState::CHARGING:
	{
		if (inputSys->IsKeyJustReleased(0x41))
		{
			m_stateController->SetState((unsigned int)ActorState::FIRE);
			break;
		}
		else if (inputSys->IsKeyPressed(0x27))
		{
			m_stateController->SetState((unsigned int)ActorState::CHARGING_WHILE_RUNNING);
			break;
		}

		break;
	}
	case (unsigned int)ActorState::CHARGING_WHILE_RUNNING:
	{
		if (inputSys->IsKeyJustReleased(0x41))
		{
			m_stateController->SetState((unsigned int)ActorState::FIRE);
			break;
		}
		else if (inputSys->IsKeyJustReleased(0x27))
		{
			m_stateController->SetState((unsigned int)ActorState::CHARGING);
			break;
		}

		m_owner->transform.Translate(Vector2(100.0f * deltaTime, 0.0f));

		break;
	}
	case (unsigned int)ActorState::FIRE:
	{
		if (inputSys->IsKeyPressed(0x27))
		{
			m_stateController->SetState((unsigned int)ActorState::RUNNING);
			break;
		}
		else if (inputSys->IsKeyPressed(0x41))
		{
			m_stateController->SetState((unsigned int)ActorState::CHARGING);
			break;
		}

		break;
	}
	}
}

void  Actor::OnStateExit(unsigned int state) 
{

}