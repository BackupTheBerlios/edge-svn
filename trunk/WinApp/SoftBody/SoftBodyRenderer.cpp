#include "StdAfx.h"
#include "./SoftBodyRenderer.hpp"

using namespace Edge;
using SoftBodyNode::TDConnectContainer;
using SoftBodyNode::TDVertexContainer;
using SoftBodyNode::TDNormalContainer;

SoftBodyRenderer::SoftBodyRenderer(void)
{
}

SoftBodyRenderer::~SoftBodyRenderer(void)
{
}

void SoftBodyRenderer::Draw(SoftBodyNode::SoftBodyNodePtr pSB) const
{
	const TDVertexContainer& Vertices(pSB->GetVertices());
	const TDConnectContainer& Connections(pSB->GetConnect());
	const TDNormalContainer& Normals(pSB->GetNormals());
	TDConnectContainer::const_iterator itConnect;
	assert(Normals.size() == Vertices.size());
	//glMatrixMode(GL_MODELVIEW);
	/*glPushMatrix();
	glLoadIdentity();*/
	for (itConnect = Connections.begin(); itConnect != Connections.end(); ++itConnect)
	{
		int Idx0((*itConnect)[0]);
		int Idx1((*itConnect)[1]);
		int Idx2((*itConnect)[2]);
		const bnu::vector<double>& Pos0(Vertices[Idx0]);
		const bnu::vector<double>& Pos1(Vertices[Idx1]);
		const bnu::vector<double>& Pos2(Vertices[Idx2]);
		const TDNormal& Normal0(*(Normals[Idx0]));
		const TDNormal& Normal1(*(Normals[Idx1]));
		const TDNormal& Normal2(*(Normals[Idx2]));
		glBegin(GL_TRIANGLES);
		glNormal3f(Normal0[0], Normal0[1], Normal0[2]);		
		glVertex3f(Pos0[0],Pos0[1], Pos0[2]);
		glNormal3f(Normal1[0], Normal1[1], Normal1[2]);		
		glVertex3f(Pos1[0],Pos1[1], Pos1[2]);
		glNormal3f(Normal2[0], Normal2[1], Normal2[2]);		
		glVertex3f(Pos2[0],Pos2[1], Pos2[2]);		
		glEnd();
	}
	//glPopMatrix();
}
