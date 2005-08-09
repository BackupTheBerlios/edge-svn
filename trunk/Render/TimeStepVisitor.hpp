#pragma once
#include "../Math/Math.hpp"
#include "./NodeVisitor.hpp"
#include "./DiffSystemNode.hpp"

template <class TSolver>
class TimeStepVisitor :
	public Edge::NodeVisitor
{
public:
			
	TimeStepVisitor(void)
	{
	}
	
	~TimeStepVisitor(void)
	{
	}
	
	void SetTime(double dt)
	{
		m_dt = dt;
	}

	double GetTime()
	{
		return dt;
	}	
	
	void Step(Edge::DiffSystemNode::DiffSystemNodePtr pDTNode)
	{
		if (pDTNode->GetDEStateSource())
			m_Solver.StepState(*(pDTNode->GetDEStateSource()), m_dt);
	}	

	virtual void Visit(Edge::MeshPtr pMesh) {}
	virtual void Visit(Edge::TranslatePtr pTrans) {}
	virtual void Visit(Edge::ScalePtr pScale) {}
	virtual void Visit(Edge::RotatePtr pRotate) {}
	virtual void Visit(Edge::BoundingBoxPtr pBox) {}
	virtual void Visit(Edge::MaterialPtr pMat) {}
	virtual void Visit(Edge::DirectionLightPtr pL) {}
	virtual void Visit(Edge::PointLightPtr pL) {}
	virtual void Visit(Edge::AmbientLightPtr pL) {}
	virtual void Visit(Edge::TextureMapPtr pT) {}
	virtual void Visit(Edge::BoundNodePtr pBNode) {}

	virtual void Leave(Edge::MeshPtr pMesh){}		
	virtual void Leave(Edge::TranslatePtr pTrans){}
	virtual void Leave(Edge::ScalePtr pScale){}
	virtual void Leave(Edge::RotatePtr pRotate){}
	virtual void Leave(Edge::BoundNodePtr pBox){}
	virtual void Leave(Edge::MaterialPtr pMat){}
	virtual void Leave(Edge::DirectionLightPtr pL){}		
	virtual void Leave(Edge::PointLightPtr pL){}
	virtual void Leave(Edge::AmbientLightPtr pL){}
	virtual void Leave(Edge::TextureMapPtr pT){}		
	
private:
	double m_dt;
	TSolver m_Solver;
};
