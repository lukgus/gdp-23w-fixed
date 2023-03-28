#include "StateMachine.h"
#include <iostream>

IdleState::IdleState(const Vector3& position, float radius)
	: State(Idle)
	, m_Position(position)
	, m_Radius(radius)
{
	printf("IdleState::IdleState\n");
}

IdleState::~IdleState()
{
	printf("IdleState::~IdleState\n");
}

void IdleState::OnEnter()
{
	printf("IdleState::OnEnter\n");
	// Send message to the BehaviourManager of this state to add a listener
}

void IdleState::OnExit()
{
	printf("IdleState::OnExit\n");
	// Send message to the BehaviourManager of this state to remove a listener
}


PursueState::PursueState(unsigned int enemyId)
	: State(Pursue)
	, m_EnemyId(enemyId)
{
	printf("PursueState::PursueState\n");
}

PursueState::~PursueState()
{
	printf("PursueState::~PursueState\n");
}

void PursueState::OnEnter()
{
	printf("PursueState::OnEnter\n");
	// Send message to the path finding to get a path to the enemy
	// Send message to the path following to control this agent to follow that path
}

void PursueState::OnExit()
{
	printf("PursueState::OnExit\n");
	// Cancel all requests for path finding & path following
}


CatchState::CatchState(unsigned int enemyId)
	: State(Catch)
	, m_EnemyId(enemyId)
{
	printf("CatchState::CatchState\n");
}

CatchState::~CatchState()
{
	printf("CatchState::~CatchState\n");
}

void CatchState::OnEnter()
{
	printf("CatchState::OnEnter\n");
	// Send message to game manager caught an enemy by id
}

void CatchState::OnExit()
{
	printf("CatchState::OnExit\n");
}

StateMachine::StateMachine()
	: m_CurrentState(nullptr)
{
	printf("StateMachine::StateMachine\n");

	m_CurrentState = new IdleState(Vector3(), 0);
}

StateMachine::~StateMachine()
{
	printf("StateMachine::~StateMachine\n");
}

void StateMachine::AddTransition(StateType from, StateType to)
{
	std::vector<StateType>& stateVec = m_ValidTransitions[from];
	if (std::find(stateVec.begin(), stateVec.end(), to) == stateVec.end())
	{
		stateVec.push_back(to);
	}
}

State* StateMachine::GetCurrentState()
{
	return m_CurrentState;
}

void StateMachine::SetState(State* state)
{
	printf("StateMachine::SetState: %s\n", state->C_Str());

	if (m_CurrentState->GetType() == state->GetType())
	{
		printf(" Already the current state!\n");
		return;
	}

	std::vector<StateType>& stateVec = m_ValidTransitions[m_CurrentState->GetType()];
	if (std::find(stateVec.begin(), stateVec.end(), state->GetType()) == stateVec.end())
	{
		printf("!! Not a valid transition!\n");
		// No valid transition was added from the curren state to the new state
		return;
	}

	m_CurrentState->OnExit();
	delete m_CurrentState;

	m_CurrentState = state;
	m_CurrentState->OnEnter();
}