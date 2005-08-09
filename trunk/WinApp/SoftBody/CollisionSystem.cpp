#include "StdAfx.h"
#include "./CollisionSystem.hpp"
#include "./SoftBodyController.hpp"
#include "../../Math/Math.hpp"

using namespace Edge;
using SoftBodyController::SoftBodyControllerPtr;
using SoftBodyController::TDParticleContainer;

void
Collide(SoftBodyNode::SoftBodyNodePtr pSB, Mesh::MeshPtr pMesh, TDForceContainer& Forces)
{
	//iterate over the particles and check for collisions with
	//the triangles of the mesh.	
	SoftBodyControllerPtr pSBC(boost::dynamic_pointer_cast<SoftBodyController>(pSB->GetDEStateSource()));
	TDForceContainer::iterator itForce;
	if (Forces.size() != pSBC->GetNumParticles())
	{
		Forces.resize(pSBC->GetNumParticles());
		for (itForce = Forces.begin();
			itForce != Forces.end();
			++itForce)
		{
			*itForce = bnu::zero_vector<double>(3);
		}
	}
	TDParticleContainer::iterator itParticle;
	for (itParticle = pSBC->GetBeginParticle(), itForce = Forces.begin();
		itParticle != pSBC->GetEndParticle();
		++itParticle, ++itForce)
	{
		Mesh::TDConnectContainer::iterator itConnect;
		for (itConnect = pMesh->GetBeginConnect();
			itConnect != pMesh->GetEndConnect();
			++itConnect)
		{
			Triangle Tri;
			LineSegment LS;
			const Mesh::TDConnect& Con(*itConnect);
			const Mesh::TDVertexContainer& Vertices(pMesh->GetVertices());
			Tri.m_P0 = Vertices[Con[0]];
			Tri.m_P1 = Vertices[Con[1]];
			Tri.m_P2 = Vertices[Con[2]];
			LS.m_P0 = (*itParticle)->GetPosition();
			const bnu::vector<double>& Velocity((*itParticle)->GetVelocity());
			LS.m_P1 = LS.m_P0 + 0.1*Velocity;
			std::vector < bnu::vector<double> > Points;
			FindIntersection(LS, Tri, Points);
			if (!Points.empty())
			{
				bnu::vector<double> Force(3);
				bnu::vector<double> Dir(LS.m_P1 - LS.m_P0);
				Force = -Dir/bnu::norm_2(Dir);
				*itForce = 50.0*Force;
				break;
			}			
		}
		if (itConnect == pMesh->GetEndConnect())
		{
			//no collisions therfore 0 force
			*itForce = bnu::zero_vector<double>(3);
		}
	}
}