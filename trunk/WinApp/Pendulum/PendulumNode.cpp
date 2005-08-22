#include "./PendulumNode.hpp"
#include "./PendulumNodeWriter.hpp"
#include "./PendulumRenderer.hpp"

using namespace Edge;
using Edge::Chain::ChainPtr;
using Edge::TDParticleVector;
using Edge::TDForceVector;
using Edge::TranslatePtr;
using std::vector;

PendulumNode::PendulumNode(void)
{
}

PendulumNode::~PendulumNode(void)
{
}

void PendulumNode::ApplyVisitor(NodeVisitor* pVisitor)
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

	PendulumRenderer* pRender = dynamic_cast<PendulumRenderer*>(pVisitor);
	if (pRender)
	{
		PendulumNodePtr pThis(boost::dynamic_pointer_cast<PendulumNode>(shared_from_this()));
		pRender->DrawPendulum(pThis);
	}	
}

void PendulumNode::Setup(Edge::Chain::ChainPtr pChain)
{
	SetDEStateSource(pChain);
	TDParticleVector::iterator itPart;
	int i = 0;
	vector<MaterialPtr> Materials;	
	for (i = 0; i < 7; ++i)
	{
		Materials.push_back(MaterialPtr(new Material));
	}
	
	//setup material properties
	Materials[0]->SetDiffuse(RGBColor(0.8, 0.2, 0.2));
	Materials[0]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[1]->SetDiffuse(RGBColor(0.8, 0.8, 0.2));
	Materials[1]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[2]->SetDiffuse(RGBColor(0.2, 0.2, 0.8));
	Materials[2]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[3]->SetDiffuse(RGBColor(0.8, 0.2, 0.8));
	Materials[3]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[4]->SetDiffuse(RGBColor(0.1, 0.5, 0.8));
	Materials[4]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[5]->SetDiffuse(RGBColor(0.5, 1.0, 0.0));
	Materials[5]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	Materials[6]->SetDiffuse(RGBColor(0.5, 0.5, 0.0));
	Materials[6]->SetAmbient(RGBColor(0.2, 0.2, 0.2));
	i=0;
	for (itPart = pChain->GetParticleBegin(); itPart != pChain->GetParticleEnd(); ++itPart, ++i)
	{
		MeshPtr pMesh(new Mesh);
		TranslatePtr pTrans(new Translate);
		pMesh->LoadObj("./sphere.obj");
		pTrans->SetTranslate((*itPart)->GetPosition());		
		ScalePtr pScale(new Scale);
		pScale->SetScale(1, 1, 1);
		m_TranslateNodes.push_back(pTrans);
		Materials[i]->AddChild(pMesh);
		pScale->AddChild(Materials[i]);
		pTrans->AddChild(pScale);
		AddChild(pTrans);
		if (i > 0)
			AddConnect(i-1, i);
	}
}

void PendulumNode::UpdateTranslateNodesFromState()
{
	Chain::StateType State;
	GetDEStateSource()->GetState(State);
	int n = m_TranslateNodes.size();
	for (int i = 0; i < n; ++i)
	{
		m_TranslateNodes[i]->SetTranslate(State[i*2]);
	}
}

void PendulumNode::SetForce(int Idx, const bnu::vector<double>& Force)
{
	Chain::ChainPtr pChain(boost::dynamic_pointer_cast<Chain>(GetDEStateSource()));
	pChain->SetForce(Idx, Force);
}

void PendulumNode::AddConnect(int Idx0, int Idx1)
{
	m_Connect.push_back(Idx0);	
	m_Connect.push_back(Idx1);	
}