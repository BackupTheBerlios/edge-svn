//#include "StdAfx.h"
#include "./Chain.hpp"
#include "./FixedPosition.hpp"
#include "./FixedDist.hpp"

using namespace Edge;
using namespace boost;
using Edge::TDParticleVector;
using Edge::TDConstraintVector;

Chain::Chain() 
{	
}

Chain::~Chain(void)
{
}

void Chain::BuildSystem()
{
	//a Chain (chain) has the first particle using a fixed position constraint 
	//and particle p+1 using a fixed distance constraint from particle p
	//because it's quicker to allocate memory before we need it, not when
	//we need it, use SetNumParticles and SetNumConstraints. The alternative is 
	//to add particles in a loop - although it's not going to matter
	//much when the number of particles is small
	
	const TDParticleVector::size_type NumParticles = m_Particles.size();
	const TDConstraintVector::size_type NumConstraints = NumParticles;
	m_CS.ResizeConstraints(NumConstraints);
	for (TDParticleVector::size_type i = 0; i < NumParticles; ++i)
	{
		m_CS.SetParticle(i, m_Particles[i]);
		if (i == 0)
		{
			//for the 1st particle use a fixed position constraint
			FixedPosition::FixedPositionPtr pFP(new FixedPosition(m_Particles[i], i, (int)NumParticles, 1));
			//pFP->SetP0(m_Particles[i]);
			//pFP->SetIdx0(i);
			//pFP->ResizeJacobian(NumParticles);
			//pFP->ResizeDJacobian(NumParticles);
			m_CS.SetConstraint(i, pFP);
		}
		else
		{
			FixedDist::FixedDistPtr pFD(new FixedDist(m_Particles[i-1], m_Particles[i], i-1, i, (int)NumParticles));
			/*pFD->SetP0(m_Particles[i-1]);
			pFD->SetP1(m_Particles[i]);
			pFD->SetIdx0(i-1);
			pFD->SetIdx1(i);
			pFD->ResizeJacobian(NumParticles);
			pFD->ResizeDJacobian(NumParticles);*/
			m_CS.SetConstraint(i, pFD);
		}
	}
	bnu::vector<double> Forces(m_Forces.size() * 3);
	TDForceVector::size_type n = m_Forces.size();
	for (int i = 0; i < n; ++i)
	{
		Forces[i*3] = m_Forces[i][0];
		Forces[i*3+1] = m_Forces[i][1];
		Forces[i*3+2] = m_Forces[i][2];
	}	
	m_CS.SetForce(Forces);
	PartConstRB::BuildSystem();
	ResizeDampingContainer(m_Particles.size());
	ResizeCollisionForceContainer(m_Particles.size());
	ResizeForceContainer(m_Particles.size());
	ResizeFrictionForceContainer(m_Particles.size());
}

//TODO: This is a temporary work around to avoid collision detection for
//systems based on chain, since the col det isn't generic enough yet
//must be removed sometime later.
void Chain::GetStateDerivative(StateType& DState)
{
	//for each partcle work out the velocity and acceleration
	TDParticleVector::size_type n = m_Particles.size();
	if (DState.size() != n*2)
	{
		DState.resize(n*2);
		for (int i = 0; i < n*2; ++i)
			DState[i].resize(3);
	}

	CalculateDamping();
	bnu::vector<double> TotalForce(m_Forces.size()*3);
	for (TDForceVector::size_type i = 0; i < m_Forces.size(); ++i)
	{
		double fx(m_Forces[i][0]);
		double fy(m_Forces[i][1]);
		double fz(m_Forces[i][2]);
		double dx(m_Damping[i][0]);
		double dy(m_Damping[i][1]);
		double dz(m_Damping[i][2]);
		double tx(fx + dx);
		double ty(fy + dy);
		double tz(fz + dz);
	
		TotalForce[i*3] = tx;
		TotalForce[i*3+1] = ty;
		TotalForce[i*3+2] = tz;
		//fstream FileForce("TotalForce.txt", ios::out | ios::app);
		//fstream FileFric("FricForce.txt", ios::out | ios::app);
		//FileForce << "Particle " << i << ": " << tx << " " << ty << " " << tz << "\n";
		//FileFric << "Particle " << i << ": " << fricx << " " << fricy << " " << fricz << "\n";
	}
	m_CS.SetForce(TotalForce);
	m_CS.CalculateConstraintForce();
	const bnu::vector<double>& CForce(m_CS.GetConstraintForce());
	for (int i = 0; i < n; ++i)
	{
		DState[i*2] = m_Particles[i]->GetVelocity();
		DState[i*2+1][0] = (TotalForce[i*3] + CForce[i*3])/m_Particles[i]->GetMass();
		DState[i*2+1][1] = (TotalForce[i*3+1] + CForce[i*3+1])/m_Particles[i]->GetMass();
		DState[i*2+1][2] = (TotalForce[i*3+2] + CForce[i*3+2])/m_Particles[i]->GetMass();					
	}
}
