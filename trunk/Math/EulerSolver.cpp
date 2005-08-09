//#include "StdAfx.h"
#include "./EulerSolver.hpp"
#include "./DEStateSource.hpp"

using namespace Edge;
using std::vector;

EulerSolver::EulerSolver(void)
{
}

EulerSolver::~EulerSolver(void)
{
}

void EulerSolver::StepState(DEStateSource& rSource, double dt)
{
	DEStateSource::StateType vecState;
	DEStateSource::StateType vecStateDerivative;	
	rSource.GetState(vecState);
	rSource.GetStateDerivative(vecStateDerivative);
	for (int i = 0; i < vecStateDerivative.size(); ++i)
	{		
		vecState[i] += dt*vecStateDerivative[i];		
	}
	rSource.SetState(vecState);
	rSource.IncTime(dt);
}
