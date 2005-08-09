//#include "StdAfx.h"
#include "./RK4Solver.hpp"
#include "./DEStateSource.hpp"

using namespace Edge;
using namespace boost::numeric;

RK4Solver::RK4Solver(void)
{
}

RK4Solver::~RK4Solver(void)
{
}

//RK4 de solver. Computes the value of the function at 
//the next time step. In practice this means it computes
//the next state of the given system at the given step size.
//For algorithm or description see any book on differential equations
//or the Witkin and Baraff notes (http://www-2.cs.cmu.edu/afs/cs.cmu.edu/user/baraff/www/sigcourse/)
//on how to write a DE solver since the architecture is based on those notes
void RK4Solver::StepState(DEStateSource& rSource, double dt)
{		
	double InitTime=0;
	DEStateSource::StateType InitState;
	DEStateSource::StateType DState1;
	DEStateSource::StateType DState2;
	DEStateSource::StateType DState3;
	DEStateSource::StateType DState4;	
	DEStateSource::StateType FinalState;
	DEStateSource::StateType InputState;
	
	InitTime = rSource.GetTime();
	rSource.GetState(InitState);
	rSource.GetStateDerivative(DState1);
	DEStateSource::StateType::size_type StateLen = InitState.size();
	FinalState.resize(StateLen);
	InputState.resize(StateLen);
	/*DState1.resize(StateLen);
	DState2.resize(StateLen);
	DState3.resize(StateLen);
	DState4.resize(StateLen);*/
	
	rSource.SetTime(InitTime + dt/2.0);
	for (int i = 0; i < StateLen; ++i)
		InputState[i] = InitState[i] + DState1[i]*dt/2.0;
	rSource.SetState(InputState);
	rSource.GetStateDerivative(DState2);
	
	rSource.SetTime(InitTime + dt/2.0);
	for (int i = 0; i < StateLen; ++i)
		InputState[i] = InitState[i] + DState2[i]*dt/2.0;
	rSource.SetState(InputState);
	rSource.GetStateDerivative(DState3);
	
	rSource.SetTime(InitTime + dt);
	for (int i = 0; i < StateLen; ++i)
		InputState[i] = InitState[i] + DState3[i]*dt;
	rSource.SetState(InputState);
	rSource.GetStateDerivative(DState4);
	
	for (int i = 0; i < StateLen; ++i)
		FinalState[i] = InitState[i] + dt/6.0*(DState1[i] + 2.0*DState2[i] + 2.0*DState3[i] + DState4[i]);

	rSource.SetState(FinalState);
}

