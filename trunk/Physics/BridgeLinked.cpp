#include "./BridgeLinked.hpp"
#include "./FixedPosition.hpp"
#include "./FixedDist.hpp"
#include "./ConstraintSystem.hpp"

using Edge::BridgeLinked;
using Edge::TDParticleVector;
using Edge::TDConstraintContainer;

BridgeLinked::BridgeLinked(void) 
{

}

BridgeLinked::~BridgeLinked(void)
{
}

void BridgeLinked::BuildSystem()
{
	SetupConstraintSystem();
}


void BridgeLinked::SetupConstraintSystem()
{
	//assert(!m_Particles.empty());
	//assert(m_Particles.size() > 2); // don't allow a brdige with less than 3 links
	//const TDParticleVector::size_type NumParticles = m_Particles.size();
	////const TDConstraintContainer::size_type NumConstraints = NumParticles + 1;
	//const TDConstraintContainer::size_type NumConstraints = NumParticles + 2;
	////const TDConstraintContainer::size_type NumConstraints = NumParticles;
	//m_CS.ResizeParticles(NumParticles);	
	//m_CS.ResizeConstraints(NumConstraints);			
	//

	////create fixed position constraints for the 1st and last particles for each side of the bridge
	//FixedPosition::FixedPositionPtr pFP0(new FixedPosition);
	//FixedPosition::FixedPositionPtr pFP1(new FixedPosition);
	//FixedPosition::FixedPositionPtr pFP2(new FixedPosition);
	//FixedPosition::FixedPositionPtr pFP3(new FixedPosition);
	//pFP0->ResizeJacobian(NumParticles);
	//pFP0->ResizeDJacobian(NumParticles);
	//pFP1->ResizeJacobian(NumParticles);
	//pFP1->ResizeDJacobian(NumParticles);
	//pFP2->ResizeJacobian(NumParticles);
	//pFP2->ResizeDJacobian(NumParticles);
	//pFP3->ResizeJacobian(NumParticles);
	//pFP3->ResizeDJacobian(NumParticles);
	//m_CS.SetParticles(m_Particles);

	//pFP0->SetP0(m_Particles[0]);
	//pFP0->SetIdx0(0);
	//m_CS.SetConstraint(0, pFP0);

	//pFP1->SetP0(m_Particles[NumParticles/2-1]);
	//pFP1->SetIdx0(NumParticles/2-1);
	//m_CS.SetConstraint(NumConstraints/2-1, pFP1);

	//pFP2->SetP0(m_Particles[NumParticles/2]);
	//pFP2->SetIdx0(NumParticles/2);
	//m_CS.SetConstraint(NumConstraints/2, pFP2);

	//pFP3->SetP0(m_Particles[NumParticles-1]);
	//pFP3->SetIdx0(NumParticles-1);
	//m_CS.SetConstraint(NumConstraints-1, pFP3);
	//
	//for (int i = 1, int ParticleIdx = 1; 
	//	i < NumConstraints/2-1; 
	//	++i, ++ParticleIdx)
	////for (int i = 1; i < NumConstraints; ++i)
	//{
	//	//fixed dist constraints for particles in between
	//	//FixedDist::FixedDistPtr pFD0(new FixedDist(m_Particles[ParticleIdx-1], m_Particles[ParticleIdx], ParticleIdx-1, ParticleIdx, NumParticles));
	//	/*pFD0->ResizeJacobian(NumParticles);
	//	pFD0->ResizeDJacobian(NumParticles);
	//	pFD0->SetP0(m_Particles[ParticleIdx-1]);
	//	pFD0->SetP1(m_Particles[ParticleIdx]);
	//	pFD0->SetIdx0(ParticleIdx-1);
	//	pFD0->SetIdx1(ParticleIdx);*/
	//	//m_CS.SetConstraint(i, pFD0);
	//}

	//for (int i = NumConstraints/2+1, int ParticleIdx = NumParticles/2+1; 
	//	i < NumConstraints-1; 
	//	++i, ++ParticleIdx)
	////for (int i = 1; i < NumConstraints; ++i)
	//{
	//	//fixed dist constraints for particles in between
	//	//FixedDist::FixedDistPtr pFD0(new FixedDist(m_Particles[ParticleIdx-1], m_Particles[ParticleIdx], 
	//	//	ParticleIdx-1, ParticleIdx, NumParticles));
	//	/*pFD0->ResizeJacobian(NumParticles);
	//	pFD0->ResizeDJacobian(NumParticles);
	//	pFD0->SetP0(m_Particles[ParticleIdx-1]);
	//	pFD0->SetP1(m_Particles[ParticleIdx]);
	//	pFD0->SetIdx0(ParticleIdx-1);
	//	pFD0->SetIdx1(ParticleIdx);*/
	//	//m_CS.SetConstraint(i, pFD0);
	//}
	//m_CS.ResizeJacobian();
	//m_CS.ResizeDJacobian();	
	//m_CS.ResizeLambda();
}

void BridgeLinked::BreakLink(int i)
{
	m_CS.RemoveConstraint(i);
	m_CS.ResizeJacobian();
	m_CS.ResizeDJacobian();	
	m_CS.ResizeLambda();
}