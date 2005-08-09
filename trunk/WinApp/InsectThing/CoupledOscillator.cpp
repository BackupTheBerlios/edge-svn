#include "StdAfx.h"
#include "./CoupledOscillator.hpp"

using namespace Edge;

using MassSpringSystem::TDPSPTuple;
using MassSpringSystem::TDPSPTupleContainer;

CoupledOscillator::CoupledOscillator(void)
{
	TDPSPTupleContainer Cont;
	Particle::Edge::ParticlePtr pP0(new Particle);
	Particle::Edge::ParticlePtr pP1(new Particle);
	Particle::Edge::ParticlePtr pP2(new Particle);
	Particle::Edge::ParticlePtr pP3(new Particle);
	pP0->SetPosition(0,0,0);
	pP1->SetPosition(0,1,0);
	pP2->SetPosition(1,0,0);
	pP3->SetPosition(1,1,0);
	Spring::SpringPtr pS0(new Spring);
	Spring::SpringPtr pS1(new Spring);
	Spring::SpringPtr pS2(new Spring);
	pS0->SetConstant(10);
	pS1->SetConstant(1000);
	pS2->SetConstant(10);
	TDPSPTuple PSP0(pP0, pS0, pP1);
	TDPSPTuple PSP1(pP1, pS1, pP3);
	TDPSPTuple PSP2(pP2, pS2, pP3);
	Cont.push_back(PSP0);
	Cont.push_back(PSP1);
	Cont.push_back(PSP2);
	m_MS.SetPSPContainer(Cont);
}

CoupledOscillator::~CoupledOscillator(void)
{
}

void CoupledOscillator::GetState(StateType& State)
{
}

void CoupledOscillator::GetStateDerivative(StateType& DState)
{

}

void CoupledOscillator::SetState(const StateType& State)
{
}