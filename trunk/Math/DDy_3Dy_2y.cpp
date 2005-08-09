#include "./DDy_3Dy_2y.hpp"

using namespace Edge;

DDy_3Dy_2y::DDy_3Dy_2y(void) :
	x0(1),
	x1(1)
{
	//y(0) = 1
	x0[0] = 1;
	//y'(0) = 1
	x1[0] = 1;
}

DDy_3Dy_2y::~DDy_3Dy_2y(void)
{
}

void DDy_3Dy_2y::GetState(StateType& States) 
{
	if (States.size() != 2)
		States.resize(2);
	States[0] = x0;
	States[1] = x1;
}

void DDy_3Dy_2y::GetStateDerivative(StateType& StateDerivatives) 
{
	if (StateDerivatives.size() != 2)
		StateDerivatives.resize(2);
		
	StateDerivatives[0] = x1;
	StateDerivatives[1] = -3.0*x1 - 2.0*x0;
}

void DDy_3Dy_2y::SetState(const StateType& State)
{
	assert(State.size() == 2);
	x0 = State[0];
	x1 = State[1];
}
