#pragma once
#include "../../Math/Math.hpp"

class CoupledOscillator :
	public Edge::DEStateSource
{
public:
	CoupledOscillator(void);
	virtual ~CoupledOscillator(void);
	
	void Setup(double k0, double k1, double k2);

	/**
	@param State The current state of the system.
	*/
	virtual void GetState(StateType& State);

	/**
	@param StateDerivative The state derivative of the system.
	*/
	virtual void GetStateDerivative(StateType& StateDerivative);

	/**
	@param State Set the state of the system
	*/
	virtual void SetState(const StateType& State);

private:
	Edge::MassSpringSystem m_MS;

};
