#pragma once

#include "../../Math/Math.hpp"
#include "../../Render/Render.hpp"
#include "../../Physics/PartConstRB.hpp"
#include "../../Physics/Force.hpp"
#include "./InsectEngine.hpp"


class InsectController :
	public Edge::PartConstRB
{
public:
	typedef boost::shared_ptr<InsectController> InsectControllerPtr;
	
	InsectController(void);
	virtual ~InsectController(void);
	virtual void BuildSystem();	
	//void BuildSystem4();
	void BuildLegs(double z);
	void BuildLegLinks();
	void BuildRoach();
	void BuildFixedAngleSystem();
	void AddForce(int Idx, double x, double y, double z);		
	virtual void GetStateDerivative(StateType& DState);
	virtual void SetState(const StateType& State);
	virtual void GetState(StateType& State);
	InsectEngine::InsectEnginePtr GetEngine() {return m_pEngine;}
	void BuildEngine();
	void Lift();	
	void SetSpringConstants();
	void SetExForceAll(const bnu::vector<double>& ExForce);
	void InsectController::CalcAllForces();
	void ResizeExForceContainer(Edge::TDForceVector::size_type n);

private:
	Edge::TDForceVector m_ExForces;
	InsectEngine::InsectEnginePtr m_pEngine;	
};
