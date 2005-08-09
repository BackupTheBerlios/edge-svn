#include "StdAfx.h"
#include "./SoftBodyController.hpp"
#include <boost/bind.hpp>

using namespace boost;
using namespace Edge;
using std::vector;
using std::list;
using SoftBodyNode::TDConnectContainer;
using SoftBodyNode::TDVertexContainer;
using SoftBodyNode::TDNormalContainer;
using SoftBodyNode::TDEdgeIdx;
using SoftBodyNode::TDConnect;
using SoftBodyNode::TDVertex;
using MassSpringSystem::TDPSPTuple;
using MassSpringSystem::TDPSPTupleContainer;
using SoftBodyNode::EdgeIdxCompare;

SoftBodyController::SoftBodyController(void)
{
}

SoftBodyController::~SoftBodyController(void)
{
}

void SoftBodyController::BuildSystem(const TDVertexContainer& Vertices, 
									 const TDConnectContainer& Connect,
									 const TDNormalContainer& Normals,
									 const bnu::vector<double>& Force)
{		
	TDVertexContainer::const_iterator itVertex;
	m_Particles.reserve(Vertices.size());	
	m_Forces.reserve(Vertices.size());
	for (itVertex = Vertices.begin(); itVertex != Vertices.end(); ++itVertex)
	{
		Edge::ParticlePtr pParticle(new Particle);
		pParticle->SetPosition(*itVertex);
		m_Particles.push_back(pParticle);
		m_Forces.push_back(Force);
	}
	BuildMassSpringSystem(Connect);	
	ResizeCollisionForceContainer(m_Particles.size());
	assert(Normals.size() == m_Particles.size());
	m_IS.SetNormalContainer(Normals);
	m_IS.SetParticleContainer(m_Particles);
}

void SoftBodyController::SetState(const StateType& State)
{
	vector<Edge::ParticlePtr>::size_type n = m_Particles.size();
	assert(State.size() == n*2);
	StateType::size_type i;
	TDParticleContainer::iterator itParticle;
	for (i = 0, itParticle = m_Particles.begin(); 
		itParticle != m_Particles.end(); 
		++i, ++itParticle)
	{
		Edge::ParticlePtr pParticle(*itParticle);
		pParticle->SetPosition(State[i*2]);
		pParticle->SetVelocity(State[i*2+1]);
	}
}

void SoftBodyController::GetState(StateType& State)
{
	int n = m_Particles.capacity();	
	if (State.capacity() != n*2)
	{
		State.reserve(n*2);
	}	
	TDParticleContainer::iterator itParticle;
	TDParticleContainer::size_type i;
	for (i = 0, itParticle = m_Particles.begin(); 
		itParticle != m_Particles.end(); 
		++itParticle, ++i)
	{
		Edge::ParticlePtr pParticle(*itParticle);
		//State[i*2] = pParticle->GetPosition();
		//State[i*2+1] = pParticle->GetVelocity();
		State.push_back(pParticle->GetPosition());
		State.push_back(pParticle->GetVelocity());
	}
}

void SoftBodyController::GetStateDerivative(StateType& StateDerivative)
{
	TDParticleContainer::iterator itParticle;
	TDForceContainer::iterator itColForce;
	TDForceContainer::iterator itForce;
	TDParticleContainer::size_type i;
	int n = m_Particles.capacity();	
	if (StateDerivative.capacity() != n*2)
	{
		StateDerivative.reserve(n*2);
	}	
	m_MS.CalculateForce();	
	m_IS.CalculateForce();
	CalculateCollisionForce();
	for (i = 0, itParticle = m_Particles.begin(), itForce = m_Forces.begin(), itColForce = m_Collision.begin(); 
		itParticle != m_Particles.end(); 
		++itParticle, ++i, ++itForce, ++itColForce)
	{
		const bnu::vector<double>& MSForce(m_MS.GetForce(*itParticle));
		const bnu::vector<double>& ISForce(m_IS.GetForce(*itParticle));
		const bnu::vector<double>& Force(*itForce);
		const bnu::vector<double>& ColForce(*itColForce);
		Edge::ParticlePtr pParticle(*itParticle);
		//StateDerivative[i*2] = pParticle->GetVelocity();
		//StateDerivative[i*2+1] = Force/pParticle->GetMass();
		StateDerivative.push_back(pParticle->GetVelocity());
		StateDerivative.push_back((Force + ISForce + MSForce + ColForce)/pParticle->GetMass());
	}
}

void SoftBodyController::BuildMassSpringSystem(const TDConnectContainer& Connect)
{
	TDConnectContainer::const_iterator itConnect;
	TDPSPTupleContainer Cont;
	TDEdgeIdx EdgeIdx;
	list<TDEdgeIdx> SpringEdgeMarker;	
	for (itConnect = Connect.begin(); itConnect != Connect.end(); ++itConnect)
	{
		TDConnect::iterator itConnectIdx;
		const TDConnect& Connect(*itConnect);
		assert(Connect.size() == 3); //should all be triangles
		Spring::SpringPtr pS0(new Spring);
		Spring::SpringPtr pS1(new Spring);
		Spring::SpringPtr pS2(new Spring);
		const TDConnect::value_type& Idx0((*itConnect)[0]);
		const TDConnect::value_type& Idx1((*itConnect)[1]);
		const TDConnect::value_type& Idx2((*itConnect)[2]);
		Edge::ParticlePtr pP0(m_Particles[Idx0]);
		Edge::ParticlePtr pP1(m_Particles[Idx1]);
		Edge::ParticlePtr pP2(m_Particles[Idx2]);		
		const TDVertex& V0(pP0->GetPosition());
		const TDVertex& V1(pP1->GetPosition());
		const TDVertex& V2(pP2->GetPosition());
		pS0->SetConstant(100);
		pS1->SetConstant(100);
		pS2->SetConstant(100);		
		pS0->SetRestLen(bnu::norm_2(V1 - V0));
		pS1->SetRestLen(bnu::norm_2(V2 - V1));
		pS2->SetRestLen(bnu::norm_2(V0 - V2));
		TDPSPTuple PSP0(pP0, pS0, pP1);
		TDPSPTuple PSP1(pP1, pS1, pP2);
		TDPSPTuple PSP2(pP2, pS2, pP0);

		EdgeIdx.first = Idx0;
		EdgeIdx.second = Idx1;		
		if (find_if(SpringEdgeMarker.begin(), SpringEdgeMarker.end(), bind(EdgeIdxCompare(), _1, EdgeIdx)) == SpringEdgeMarker.end())
		{
			//not yet added so add a spring between this edge
			SpringEdgeMarker.push_back(EdgeIdx);
			Cont.push_back(PSP0);
		}
		

		EdgeIdx.first = Idx1;
		EdgeIdx.second = Idx2;		
		if (find_if(SpringEdgeMarker.begin(), SpringEdgeMarker.end(), bind(EdgeIdxCompare(), _1, EdgeIdx)) == SpringEdgeMarker.end())
		{
			//not yet added so add a spring between this edge
			SpringEdgeMarker.push_back(EdgeIdx);
			Cont.push_back(PSP1);
		}

		EdgeIdx.first = Idx2;
		EdgeIdx.second = Idx0;		
		if (find_if(SpringEdgeMarker.begin(), SpringEdgeMarker.end(), bind(EdgeIdxCompare(), _1, EdgeIdx)) == SpringEdgeMarker.end())
		{
			//not yet added so add a spring between this edge
			SpringEdgeMarker.push_back(EdgeIdx);
			Cont.push_back(PSP2);
		}
	}
	m_MS.SetPSPContainer(Cont);
}


bool SoftBodyController::CheckForCollision(Edge::ParticlePtr pP, double dt, double& t)
{
	//check if particle will collide with ground plane at the next time step. Not exact.
	const bnu::vector<double>& Pos0(pP->GetPosition());
	bnu::vector<double> Pos1(Pos0 + (pP->GetVelocity()*dt));
	bnu::vector<double> Normal(3); //normal of plane perpendicular to y axis
	Normal[1] = 1;
	return LineSegPlaneInt(Pos0, Pos1, Normal, -1, t);
	
}

void SoftBodyController::CalculateCollisionForce()
{
	TDParticleContainer::iterator itParticle;
	int i = 0;
	for (itParticle = m_Particles.begin(); itParticle != m_Particles.end(); ++itParticle, ++i)
	{
		Edge::ParticlePtr pP(*itParticle);
		double t;
		double dt = 0.01;
		double ks = 1000;
		if (CheckForCollision(pP, dt, t) || pP->GetPosition()[1] <= -1)
		{
			const bnu::vector<double>& Pos0(pP->GetPosition());
			bnu::vector<double> Pos1(Pos0 + (pP->GetVelocity()*dt));
			bnu::vector<double> Acceleration(3);
			const bnu::vector<double>& Velocity(pP->GetVelocity());
			bnu::vector<double> Normal(3);
			Normal[0] = 0; Normal[1] = 1; Normal[2] = 0; 
			double d = bnu::inner_prod(Pos1, Normal) + 1; //plane is at -1 y - get penetration depth
			//penalty force
			double fx(-ks*d*Normal[0]); 
			double fy(-ks*d*Normal[1]);
			double fz(-ks*d*Normal[2]);
			m_Collision[i][0] = fx;
			m_Collision[i][1] = fy;
			m_Collision[i][2] = fz;
		}
		else
		{
			m_Collision[i][0] = 0;
			m_Collision[i][1] = 0;
			m_Collision[i][2] = 0;
		}
	}
}

void SoftBodyController::ResizeCollisionForceContainer(TDForceContainer::size_type Size)
{
	m_Collision.resize(Size);
	for (int i = 0; i < Size; ++i)
	{
		m_Collision[i].resize(3);		
	}
}

void 
SoftBodyController::AddCollisionForces(const TDForceContainer& Forces)
{
	int s0 = m_Collision.size();
	int s1 = Forces.size();
	assert(m_Collision.size() == Forces.size());
	TDForceContainer::const_iterator itForce;
	TDForceContainer::iterator itCollision;
	for (itForce = Forces.begin(), itCollision = m_Collision.begin();
		itForce != Forces.end();
		++itForce, ++itCollision)
	{
		*itCollision = *itCollision + *itForce;
	}
}

void
SoftBodyController::ZeroCollisionForces()
{
	TDForceContainer::iterator itColForce;
	for (itColForce = m_Collision.begin();
		itColForce != m_Collision.end();
		++itColForce)
	{
		*itColForce = bnu::zero_vector<double>(3);
	}
}