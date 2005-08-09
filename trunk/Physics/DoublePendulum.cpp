//#include "StdAfx.h"
#include "./DoublePendulum.hpp"

using namespace Edge;

DoublePendulum::DoublePendulum(void)
{
}

DoublePendulum::~DoublePendulum(void)
{
}

void DoublePendulum::GetState(StateType& State) const 
{

}

void DoublePendulum::GetStateDerivative(StateType& StateDerivative) const 
{
	StateDerivate.resize(4); //2 particles (vel, acc, vel, acc)

}

void DoublePendulum::SetState(const StateType& State)
{
	
}
