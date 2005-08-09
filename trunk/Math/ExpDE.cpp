//#include "StdAfx.h"
#include "./ExpDE.hpp"

using namespace Edge;

ExpDE::ExpDE() :
	x(1)
{
	x[0] = 1;
}

ExpDE::~ExpDE()
{

}
void ExpDE::GetState(StateType& State)
{
	if (State.size() != 1)
		State.resize(1);
	State[0] = x;
}

void ExpDE::GetStateDerivative(StateType& State)
{
	//representing eqn: y' = y, y(0) = 1
	if (State.size() != 1)
		State.resize(1);
	State[0] = x;
}

void ExpDE::SetState(const StateType& State)
{
	assert(State.size() == 1);
	x = State[0];
}
