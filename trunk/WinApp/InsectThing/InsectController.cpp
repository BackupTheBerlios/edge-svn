#include "StdAfx.h"
#include "../../Physics/FixedDist.hpp"
#include "../../Physics/FixedAngle.hpp"
#include "../../Physics/FixedPlane.hpp"
#include "../../Physics/Particle.hpp"
#include "../../Math/MatrixSupport.hpp"
#include "./InsectController.hpp"

using namespace Edge;
using Edge::Particle;
using Edge::MassSpringSystem;
using Edge::TDForceVector;
using Edge::TDParticleVector;
using MassSpringSystem::TDPSPTupleContainer;
using MassSpringSystem::TDPSPTuple;
using Edge::FixedDist;
using Edge::FixedPlane;

InsectController::InsectController(void) :
	m_pEngine(new InsectEngine)
{
}

InsectController::~InsectController(void)
{
}

void InsectController::BuildSystem(void)
{
//	BuildSystem4(); 
	BuildRoach();
//	BuildFixedAngleSystem();
	PartConstRB::BuildSystem();
}

//void InsectController::BuildSystem4()
//{
//	Particle::Edge::ParticlePtr pP0(new Particle);
//	ResizeParticleContainer(m_Particles.size());
//	pP0->SetPosition(0,-1,0);
//	m_Particles.push_back(pP0);
//	
//	TDParticleVector::size_type NumParticles(m_Particles.size());
////	m_CS.ResizeParticles(m_Particles.size());
//	bnu::vector<double> Forces(3);	
//	Forces[0] = 0; Forces[1] = -10; Forces[2] = 0;
//	m_CS.SetParticles(m_Particles);
//	ResizeForceContainer(m_Particles.size());
//	ResizeCollisionForceContainer(m_Particles.size());
//	ResizeFrictionForceContainer(m_Particles.size());
//	ResizeDampingContainer(m_Particles.size());
//	SetForceAllParticles(Forces);
//}


void InsectController::SetState(const StateType& State)
{
	StateType::size_type ParticleStateCount = m_Particles.size() * 2;
	assert(State.size() == ParticleStateCount);	
	StateType::size_type i;
	for (i = 0; i < ParticleStateCount/2; ++i)
	{
		m_Particles[i]->SetPosition(State[i*2]);
		m_Particles[i]->SetVelocity(State[i*2+1]);
	}	

}

void InsectController::GetState(StateType& State)
{
	StateType::size_type ParticleStateCount = m_Particles.size()*2;		
	//StateType::size_type AngleCount = m_pEngine->GetSpringCount();
	//StateType::size_type StateCount = ParticleStateCount + AngleCount;
	StateType::size_type StateCount = ParticleStateCount;
	StateType::size_type i;
	if (State.size() != StateCount)
	{
		State.resize(StateCount);
		for (i = 0; i < ParticleStateCount; ++i)
		{
			//3d vectors representing position, velocity, 
			State[i].resize(3);
		}

		for (; i < StateCount; ++i)
		{
			State[i].resize(1);
		}
	}
	for (i = 0; i < ParticleStateCount/2; ++i)
	{
		assert(State[i].size() == 3);		
		State[i*2] = m_Particles[i]->GetPosition();				
		State[i*2+1] = m_Particles[i]->GetVelocity();
	}
}

void 
InsectController::CalcAllForces()
{
	assert(m_ExForces.size() == m_Particles.size());
	assert(m_Forces.size() == m_Particles.size());
	TDParticleVector::iterator itParticle;
	for (itParticle = m_Particles.begin();
		itParticle != m_Particles.end();
		++itParticle)
	{
		m_pEngine->CalcForce(*itParticle);
	}
	
	TDForceVector::iterator itForce;
	TDForceVector::iterator itExForce;
	for (itForce = m_Forces.begin(), itExForce = m_ExForces.begin(), itParticle = m_Particles.begin();
		itForce != m_Forces.end();
		++itForce, ++itExForce, ++itParticle)
	{
		*itForce = *itExForce + m_pEngine->GetForce(*itParticle);
	}
}

void
InsectController::GetStateDerivative(StateType& DState) 
{
	//for each partcle work out the velocity and acceleration
	StateType::size_type ParticleStateCount = m_Particles.size()*2;	
	StateType::size_type StateCount = ParticleStateCount;
	if (DState.size() != StateCount)
	{
		DState.resize(StateCount);
		for (StateType::size_type i = 0; i < ParticleStateCount; ++i)
		{
			//3d vectors representing position, velocity, 
			DState[i].resize(3);
		}
		for (i = ParticleStateCount; i < StateCount; ++i)
		{
			DState[i].resize(1);
		}
	}

	CalcAllForces();

	CalculateDamping();
	CalculateCollisionForce();
	CalculateFrictionForce();
	TDParticleVector::iterator itParticle;
	
	bnu::vector<double> TotalForce(m_Forces.size()*3);
	//calculate the total force on each particle in the system 
	//then calculate the constraint force.
	for (TDForceVector::size_type i = 0; i < m_Forces.size(); ++i)
	{
		double fx(m_Forces[i][0]);
		double fy(m_Forces[i][1]);
		double fz(m_Forces[i][2]);
		double fricx(m_Friction[i][0]);
		double fricy(m_Friction[i][1]);
		double fricz(m_Friction[i][2]);
		double dx(m_Damping[i][0]);
		double dy(m_Damping[i][1]);
		double dz(m_Damping[i][2]);
		double cx(m_Collision[i][0]);
		double cy(m_Collision[i][1]);
		double cz(m_Collision[i][2]);		
		double tx(fx + dx + cx + fricx);
		double ty(fy + dy + cy + fricy);
		double tz(fz + dz + cz + fricz);
				
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

	//calculate the acceleration of the particles in the system
	for (int i = 0; i < ParticleStateCount/2; ++i)
	{
		assert(DState[i*2].size() == 3);
		DState[i*2] = m_Particles[i]->GetVelocity();
		double c0(CForce[i*3]);
		double c1(CForce[i*3+1]);
		double c2(CForce[i*3+2]);
		double tx(TotalForce[i*3]);
		double ty(TotalForce[i*3+1]);
		double tz(TotalForce[i*3+2]);
		assert(DState[i*2+1].size() == 3);
		assert(m_Particles[i]->GetMass() > 1e-7);
		DState[i*2+1][0] = (tx + c0)/m_Particles[i]->GetMass();
		DState[i*2+1][1] = (ty + c1)/m_Particles[i]->GetMass();
		DState[i*2+1][2] = (tz + c2)/m_Particles[i]->GetMass();		
		/*DState[i*2+1][0] = (tx)/m_Particles[i]->GetMass();
		DState[i*2+1][1] = (ty)/m_Particles[i]->GetMass();
		DState[i*2+1][2] = (tz)/m_Particles[i]->GetMass();*/
	}
	
	////angular velocity
	//for (i = ParticleStateCount; i < StateCount; ++i)
	//{
	//	DState[i][0] = 1;
	//}
}

void InsectController::BuildLegs(double z)
{
	//1st set of legs	
	TDParticleVector Temp;
	Temp.resize(6);
	TDParticleVector::iterator it;
	for (it = Temp.begin(); it != Temp.end(); ++it)
	{
		*it = Edge::ParticlePtr(new Particle);
	}
	
	Temp[0]->SetPosition(-2.2,-1,z);
	Temp[1]->SetPosition(-2,1,z);	
	Temp[2]->SetPosition(0,0,z);
	//Temp[3]->SetPosition(0.5,1,z);
	Temp[3]->SetPosition(1,0,z);
	Temp[4]->SetPosition(3,1,z);
	Temp[5]->SetPosition(3.2,-1,z);	

	TDParticleVector::size_type Offset(m_Particles.size());
	m_Particles.insert(m_Particles.end(), Temp.begin(), Temp.end());
	int NumParticles(m_Particles.size());	
	//TODO: Fix this awful hack regarding the NumParticles parameter. It's actually quite hard and
	//will involve a rethink of the way the constraint system currently builds its jacobian.
	FixedDist::FixedDistPtr pFD0(new FixedDist(Temp[0], Temp[1], Offset + 0, Offset + 1, 18));
	FixedDist::FixedDistPtr pFD1(new FixedDist(Temp[1], Temp[2], Offset + 1, Offset + 2, 18));
	FixedDist::FixedDistPtr pFD2(new FixedDist(Temp[2], Temp[3], Offset + 2, Offset + 3, 18));
	FixedDist::FixedDistPtr pFD3(new FixedDist(Temp[3], Temp[4], Offset + 3, Offset + 4, 18));
	FixedDist::FixedDistPtr pFD4(new FixedDist(Temp[4], Temp[5], Offset + 4, Offset + 5, 18));
	//FixedDist::FixedDistPtr pFD5(new FixedDist(Temp[5], Temp[6], Offset + 5, Offset + 6, 21));
	bnu::vector<double> u(Temp[1]->GetPosition() - Temp[2]->GetPosition());
	bnu::vector<double> v(Temp[3]->GetPosition() - Temp[2]->GetPosition());
	bnu::vector<double> n(3);
	Cross(u,v,n);
	double d = bnu::inner_prod(n, Temp[1]->GetPosition());
	FixedPlane::FixedPlanePtr pFP0(new FixedPlane(n, d, Temp[0], Offset, 18));
	m_CS.AddConstraint(pFD0);
	m_CS.AddConstraint(pFD1);
	m_CS.AddConstraint(pFD2);
	m_CS.AddConstraint(pFD3);
	m_CS.AddConstraint(pFD4);
	m_CS.AddConstraint(pFP0);	
}

void InsectController::BuildLegLinks()
{
	TDParticleVector::size_type NumParticles(m_Particles.size());
	int LegSets(m_Particles.size()/6);
	for (int i = 0; i < LegSets-1; ++i)
	{
		int Idx0(i*6+2);
		int Idx1((i+1)*6+2);
		int Idx2(i*6+3);
		int Idx3((i+1)*6+3);
		FixedDist::FixedDistPtr pFD0(new FixedDist(m_Particles[Idx0], m_Particles[Idx1], Idx0, Idx1, NumParticles));
		FixedDist::FixedDistPtr pFD1(new FixedDist(m_Particles[Idx2], m_Particles[Idx3], Idx2, Idx3, NumParticles));
		m_CS.AddConstraint(pFD0);
		m_CS.AddConstraint(pFD1);
	}
}

void InsectController::BuildEngine()
{
	/*TDParticleVector::iterator itParticle;
	InsectEngine::AntiGravForce AGF;
	bnu::vector<double> F(bnu::zero_vector<double>(3));
	F[1] = 1;
	AGF.m_Force = F;
	for (itParticle = m_Particles.begin();
		itParticle != m_Particles.end();
		++itParticle)
	{
		m_pEngine->AddAntiGravForce(*itParticle, AGF);
	}*/
	m_pEngine->Setup(m_Particles);
}

void
InsectController::BuildRoach()
{
//	ResizeParticleContainer(m_Particles.size());		
	BuildLegs(0);		
	BuildLegs(2);
	BuildLegs(4);
	BuildLegLinks();
	BuildEngine();
	bnu::vector<double> Forces(3);
	Forces[0] = 0; Forces[1] = -1; Forces[2] = 0;
	m_CS.SetParticles(m_Particles);	
//	m_CS.ResizeDJacobian();
//	m_CS.ResizeJacobian();
	int NumParticles(m_Particles.size());
	ResizeForceContainer(NumParticles);
	ResizeExForceContainer(NumParticles);
	ResizeCollisionForceContainer(NumParticles);
	ResizeFrictionForceContainer(NumParticles);
	ResizeDampingContainer(NumParticles);
	//SetForceAllParticles(Forces);
	SetExForceAll(Forces);
}

void
InsectController::BuildFixedAngleSystem()
{
	Edge::ParticlePtr pP0(new Particle);
	Edge::ParticlePtr pP1(new Particle);
	Edge::ParticlePtr pP2(new Particle);
	pP0->SetPosition(-0.3,1,0);
	pP1->SetPosition(0,0,0);
	pP2->SetPosition(1,0,0);
	m_Particles.push_back(pP0);
	m_Particles.push_back(pP1);
	m_Particles.push_back(pP2);
	int NumParticles(m_Particles.size());
	
	FixedDist::FixedDistPtr pFD0(new FixedDist(pP0, pP1, 0, 1, NumParticles));
	FixedDist::FixedDistPtr pFD1(new FixedDist(pP1, pP2, 1, 2, NumParticles));
	FixedAngle::FixedAnglePtr pFA(new FixedAngle(pP0, pP1, pP2, 0, 1, 2, NumParticles));	

	m_CS.AddConstraint(pFD0);
	m_CS.AddConstraint(pFD1);
	m_CS.AddConstraint(pFA);
	m_CS.SetParticles(m_Particles);
	bnu::vector<double> Forces(3);
	Forces[0] = 0; Forces[1] = -1; Forces[2] = 0;
	
	ResizeForceContainer(NumParticles);
	ResizeCollisionForceContainer(NumParticles);
	ResizeFrictionForceContainer(NumParticles);
	ResizeDampingContainer(NumParticles);
	SetForceAllParticles(Forces);
}

void
InsectController::ResizeExForceContainer(TDForceVector::size_type n)
{
	m_ExForces.resize(n);
}

void 
InsectController::SetExForceAll(const bnu::vector<double>& Force)
{
	TDForceVector::iterator itForce;
	for (itForce = m_ExForces.begin();
		itForce != m_ExForces.end();
		++itForce)
	{
		*itForce = Force;
	}
}
