#include "StdAfx.h"
#include "./MassSpringNode.hpp"

using namespace Edge;
using Edge::MassSpringSystem::MassSpringSystemPtr;
using Edge::MassSpringSystem::TDParticleContainer;
using Edge::MassSpringSystem::TDSpringContainer;

MassSpringNode::MassSpringNode(void)
{
}

MassSpringNode::~MassSpringNode(void)
{
}

void MassSpringNode::Setup(MassSpringSystemPtr pMS)
{
	TDParticleContainer::iterator itPart;
	SetDiffSystem(pMS);
	for (itPart = pMS->GetParticleBegin(); itPart != pMS->GetParticleEnd(); ++itPart)
	{
		Mesh::MeshPtr pMesh(new Mesh);
		Translate::TranslatePtr pTrans(new Translate);
		pMesh->LoadObj("./sphere.obj");
		pTrans->SetTranslate((*itPart)->GetPosition());		
		m_TranslateNodes.push_back(pTrans);
		pTrans->AddChild(pMesh);
		AddChild(pTrans);
	}
}

void MassSpringNode::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	DiffSystemNode::ApplyVisitor(pVisitor);
	//update the translate nodes with the new states
	UpdateTranslateNodesFromState();	
}

void MassSpringNode::UpdateTranslateNodesFromState()
{
	MassSpringSystem::StateType State;
	GetDiffSystem()->GetState(State);
	int n = m_TranslateNodes.size();
	for (int i = 0; i < n; ++i)
	{
		m_TranslateNodes[i]->SetTranslate(State[i*2]);
	}
}