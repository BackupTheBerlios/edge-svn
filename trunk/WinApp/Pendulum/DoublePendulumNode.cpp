#include "StdAfx.h"
#include "./DoublePendulumNode.hpp"
#include "./PendulumNodeWriter.hpp"

using namespace Edge;

DoublePendulumNode::DoublePendulumNode(void)
{
}

DoublePendulumNode::~DoublePendulumNode(void)
{
}

void DoublePendulumNode::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	DiffSystemNode::ApplyVisitor(pVisitor);	
	//update the translate nodes with the new states
	UpdateTranslateNodesFromState();	
	if (typeid(*pVisitor) == typeid(PendulumNodeWriter))
	{
		PendulumNodeWriter* pWriter = dynamic_cast<PendulumNodeWriter*>(pVisitor);
		for (int i = 0; i < m_TranslateNodes.size(); ++i)
		{			
			pWriter->WriteTranslate(*m_TranslateNodes[i], i);
		}
	}
}

//void DoublePendulumNode::ConvertAngularToCartesian(double& x0, double& y0, double& x1, double& y1)
//{
//	DoublePendulum::DoublePendulumPtr pDblPen(boost::dynamic_pointer_cast<DoublePendulum>(GetDiffSystem()));
//	DoublePendulum::StateType State;
//	pDblPen->GetState(State);
//	double L0 = pDblPen->GetL0();
//	double L1 = pDblPen->GetL1();
//	double Theta0 = State[0][0];
//	double DTheta0 = State[0][1];
//	double Theta1 = State[1][0];
//	double DTheta1 = State[1][1];
//	
//	x0 = L0*sin(Theta0);
//	y0 = -L0*cos(Theta0);
//	x1 = x0 + L1*sin(Theta1);
//	y1 = y0 - L1*cos(Theta1);
//}

void DoublePendulumNode::UpdateTranslateNodesFromState()
{
	double x0, y0, x1, y1;
	DoublePendulum::DoublePendulumPtr pDblPen(boost::dynamic_pointer_cast<DoublePendulum>(GetDiffSystem()));
	//ConvertAngularToCartesian(x0, y0, x1, y1);
	//bnu::vector<double> T(3);
	int n = m_TranslateNodes.size();
	assert(n == 2);
	m_TranslateNodes[0]->SetTranslate(pDblPen->GetParticle0()->GetPosition());
	m_TranslateNodes[1]->SetTranslate(pDblPen->GetParticle1()->GetPosition());
}

void DoublePendulumNode::Setup(Edge::DoublePendulum::DoublePendulumPtr pDblPen)
{
	SetDiffSystem(pDblPen);
	Mesh::MeshPtr pMesh0(new Mesh);
	Mesh::MeshPtr pMesh1(new Mesh);
	Translate::TranslatePtr pTrans0(new Translate);
	Translate::TranslatePtr pTrans1(new Translate);
	Particle::ParticlePtr pP0(pDblPen->GetParticle0());
	Particle::ParticlePtr pP1(pDblPen->GetParticle1());
	pMesh0->LoadObj("./sphere.obj");
	pMesh1->LoadObj("./sphere.obj");
	pTrans0->SetTranslate(pP0->GetPosition());
	pTrans1->SetTranslate(pP1->GetPosition());
	m_TranslateNodes.push_back(pTrans0);
	m_TranslateNodes.push_back(pTrans1);
	pTrans0->AddChild(pMesh0);
	AddChild(pTrans0);
	pTrans1->AddChild(pMesh1);
	AddChild(pTrans1);
}
