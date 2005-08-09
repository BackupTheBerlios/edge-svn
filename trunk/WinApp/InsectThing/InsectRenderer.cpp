#include "StdAfx.h"
#include "./InsectRenderer.hpp"

using namespace Edge;
using Insect::TDConnectContainer;

InsectRenderer::InsectRenderer(void)
{
}

InsectRenderer::~InsectRenderer(void)
{
}

void InsectRenderer::DrawInsect(Insect::InsectPtr pInsect) const
{
	DEStateSource::DEStateSourcePtr pDEState(pInsect->GetDEStateSource());
	DEStateSource::StateType State;
	pDEState->GetState(State);
	DEStateSource::StateType::iterator it(State.begin());
	//assert(State.size() >= 4);	
	const TDConnectContainer& Connect(pInsect->GetFDConnect());
	TDConnectContainer::const_iterator itConnect;
	for (itConnect = Connect.begin(); itConnect != Connect.end(); ++itConnect)
	{
		Edge::ParticlePtr pP0(itConnect->get<0>());
		Edge::ParticlePtr pP1(itConnect->get<1>());
		const bnu::vector<double>& Pos0(pP0->GetPosition());
		const bnu::vector<double>& Pos1(pP1->GetPosition());
		float rgb[3];
		rgb[0] = 0.0;
		rgb[1] = 0.0;
		rgb[2] = 0.7;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
		glBegin(GL_LINES);
		glVertex3f(Pos0[0],Pos0[1], Pos0[2]);
		glVertex3f(Pos1[0],Pos1[1], Pos1[2]);
		glEnd();
	}
	
	const TDConnectContainer& SpringConnect(pInsect->GetSpringConnect());
	TDConnectContainer::const_iterator itSpringConnect;
	for (itSpringConnect = SpringConnect.begin(); 
		itSpringConnect != SpringConnect.end(); 
		++itSpringConnect)
	{
		Edge::ParticlePtr pP0(itSpringConnect->get<0>());
		Edge::ParticlePtr pP1(itSpringConnect->get<1>());
		const bnu::vector<double>& Pos0(pP0->GetPosition());
		const bnu::vector<double>& Pos1(pP1->GetPosition());
		float rgb[3];
		rgb[0] = 0.7f;
		rgb[1] = 0.7f;
		rgb[2] = 0.7f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rgb);
		glBegin(GL_LINES);
		glVertex3f((GLfloat)Pos0[0], (GLfloat)Pos0[1], (GLfloat)Pos0[2]);
		glVertex3f((GLfloat)Pos1[0],(GLfloat)Pos1[1], (GLfloat)Pos1[2]);
		glEnd();
	}
}