#pragma once

#include "../../Render/Render.hpp"
#include "./SoftBodyNode.hpp"

typedef std::vector< bnu::vector<double> > TDForceContainer;

/**
Collide a soft body node with a mesh. Return forces to apply to the particles
of the softbody system to simulate the collision.
*/
void
Collide(SoftBodyNode::SoftBodyNodePtr pSB, Edge::Mesh::MeshPtr pMesh, TDForceContainer& Forces);
