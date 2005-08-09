#include "StdAfx.h"
#include "../../Physics/FixedDist.hpp"
#include "../../Render/NodeVisitor.hpp"
#include "../../Render/Mesh.hpp"
#include "../../Render/Material.hpp"
#include "../../Render/RGBColor.hpp"
#include "../../Render/Translate.hpp"
#include "../../Render/Scale.hpp"
#include "../../Physics/PartConstRB.hpp"
#include "./Insect.hpp"
#include "./InsectRenderer.hpp"

using namespace Edge;
//using Edge::Translate;
//using Edge::Scale;
//using Edge::RGBColor;
//using Edge::Material;
//using Edge::Mesh;
//using Edge::NodeVisitor;
//using Edge::FixedDist;
//using Edge::TDParticleVector;
//using Edge::TDForceVector;
//using Edge::ConstraintSystem;
//using Edge::PartConstRB;
using Edge::ConstraintSystem::TDConstraintContainer;
using Edge::MassSpringSystem::TDPSPTupleContainer;

Insect::Insect(void)
{
}

Insect::~Insect(void)
{
}

void Insect::ApplyVisitor(NodeVisitor* pVisitor)
{
	InsectRenderer* pIRender = dynamic_cast<InsectRenderer*>(pVisitor);
	if (pIRender)
	{
		InsectPtr pThis(boost::dynamic_pointer_cast<Insect>(shared_from_this()));
		pIRender->DrawInsect(pThis);
	}
	DiffSystemNode::ApplyVisitor(pVisitor);	
	//update the translate nodes with the new states
	UpdateTranslateNodesFromState();	
}

void Insect::Setup(InsectController::InsectControllerPtr pInsCont)
{
	SetDEStateSource(pInsCont);
	TDParticleVector::iterator itPart;
	for (itPart = pInsCont->GetParticleBegin(); itPart != pInsCont->GetParticleEnd(); ++itPart)
	{
		Edge::MeshPtr pMesh(new Mesh);
		Material::MaterialPtr pMat(new Material);
		//setup material properties
		pMat->SetDiffuse(RGBColor(0.8, 0.1, 0.1));
		pMat->SetAmbient(RGBColor(0.2, 0.0, 0.0));		
		Edge::TranslatePtr pTrans(new Translate);
		ScalePtr pScale(new Scale);		
		pScale->SetScale(0.2, 0.2, 0.2);
		pMesh->LoadObj("./sphere.obj");
		pTrans->SetTranslate((*itPart)->GetPosition());		
		m_TranslateNodes.push_back(pTrans);
		pScale->AddChild(pMesh);
		pTrans->AddChild(pScale);
		pMat->AddChild(pTrans);
		AddChild(pMat);
	}

	//create connection list so the renderer knows where to draw lines
	//1st set of legs
//	BuildSystem1();
	BuildRoachConnectPoints(pInsCont);
}

void Insect::BuildRoachConnectPoints(InsectController::InsectControllerPtr pInsCont)
{
	TDConstraintContainer::const_iterator itCon;
	const ConstraintSystem& CS(pInsCont->GetConstraintSystem());

	for (itCon = CS.GetConstraintBegin(); 
		itCon != CS.GetConstraintEnd();
		++itCon)
	{		
		FixedDist::FixedDistPtr pFD(boost::dynamic_pointer_cast<FixedDist>(*itCon));
		if (pFD)
		{
			TDParticleTuple Particles(pFD->GetP0(), pFD->GetP1());
			m_FDConnect.push_back(Particles);
		}
	}

}


void Insect::UpdateTranslateNodesFromState()
{
	PartConstRB::StateType State;
	GetDEStateSource()->GetState(State);
	TDTranslateContainer::size_type n = m_TranslateNodes.size();
	for (TDTranslateContainer::size_type i = 0; i < n; ++i)
	{
		m_TranslateNodes[i]->SetTranslate(State[i*2]);
	}
}

void Insect::SetForce(int Idx, const bnu::vector<double>& Force)
{
	InsectController::InsectControllerPtr pInsCont(boost::dynamic_pointer_cast<InsectController>(GetDEStateSource()));
	pInsCont->SetForce(Idx, Force);
}


void Insect::Accelerate()
{
	InsectController::InsectControllerPtr pInsCont(boost::dynamic_pointer_cast<InsectController>(GetDEStateSource()));
	bnu::vector<double> Force(3);
	Force[0] = 0; Force[1] = 0; Force[2] = 1;
	pInsCont->SetForce(0, Force);
	pInsCont->SetForce(4, Force);
}

void Insect::Decelerate()
{
	InsectController::InsectControllerPtr pInsCont(boost::dynamic_pointer_cast<InsectController>(GetDEStateSource()));
	bnu::vector<double> Force(3);
	Force[0] = 0; Force[1] = 0; Force[2] = -1;
	pInsCont->SetForce(0, Force);
	pInsCont->SetForce(4, Force);
}

void Insect::ZeroAcceleration()
{
	InsectController::InsectControllerPtr pInsCont(boost::dynamic_pointer_cast<InsectController>(GetDEStateSource()));
	bnu::vector<double> Force(3);
	Force[0] = 0; Force[1] = 0; Force[2] = 0;
	pInsCont->SetForce(0, Force);
	pInsCont->SetForce(4, Force);
}

