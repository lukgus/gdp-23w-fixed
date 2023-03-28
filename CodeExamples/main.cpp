#include "StateMachine.h"

void StateMachineExample()
{
	Vector3 position;
	StateMachine stateMachine;

	stateMachine.AddTransition(Idle, Pursue);
	stateMachine.AddTransition(Pursue, Idle);
	stateMachine.AddTransition(Pursue, Catch);
	stateMachine.AddTransition(Catch, Idle);

	stateMachine.SetState(new IdleState(position, 1));
	stateMachine.SetState(new PursueState(1));
	stateMachine.SetState(new CatchState(1));
	stateMachine.SetState(new IdleState(position, 1));
	stateMachine.SetState(new CatchState(1));
	stateMachine.SetState(new PursueState(1));
}

class Test
{
public:
	Test() {
		printf("Test was created!\n");
	}
};

using namespace std;
typedef map<int, Test> int_map;
typedef int_map::iterator int_map_iter;
void MapExample()
{
	int_map intMap;
	int_map_iter it;

	it = intMap.find(1);
	if (it == intMap.end())
	{
		printf("Not found!\n");
	}

	Test result = intMap[1];

	it = intMap.find(1);
	if (it == intMap.end())
	{
		printf("Not found!\n");
	}
	else
	{
		printf("Found!");
	}
}

int main(int argc, char** argv)
{
	StateMachineExample();

	return 0;
}