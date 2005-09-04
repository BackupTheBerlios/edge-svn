#include "./RigidBody.hpp"
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "../Math/MatrixSupport.hpp"

using namespace Edge;

RigidBody::RigidBody() :
	m_Position(3),
	m_CountStateVectors(6),
	m_Rotate(3,3),
	m_LinearMomentum(3),
	m_AngularMomentum(3)
{

}

void
RigidBody::GetState(StateType& State)
{
	// Copy the elements of the member variables that describe the state of 
	// the rigid body into the state vector.
	StateType::iterator it = State.begin();
	if (State.size() != m_CountStateVectors)
	{
		State.resize(m_CountStateVectors);
	}
	*it++ = m_Position;
	*it++ = bnu::column(m_Rotate, 0);
	*it++ = bnu::column(m_Rotate, 1);
	*it++ = bnu::column(m_Rotate, 2);
	*it++ = m_AngularMomentum;
	*it = m_LinearMomentum;
}

void
RigidBody::GetStateDerivative(StateType& State)
{
	//Work out the state derivatives and place them in State
	StateType::iterator it = State.begin();
	bnu::vector<double> AngularMomentum;
	bnu::vector<double> Temp0;
	bnu::vector<double> Temp1;
	if (State.size() != m_CountStateVectors)
	{
		State.resize(m_CountStateVectors);
	}
	bnu::vector<double> LinearVelocity(m_LinearMomentum/m_Mass);

	/* Inertia Tensor = R*IBodyInv*RTranspose*/
	bnu::matrix<double> InertiaTensorInv((bnu::prod(m_Rotate, m_BodySpaceInertiaTensorInv)));
	InertiaTensorInv = bnu::prod(InertiaTensorInv, bnu::trans(m_Rotate));

	/* omega = IInv * angularmomentum*/
	bnu::vector<double> Omega(bnu::prod(InertiaTensorInv, m_AngularMomentum));

	*it++ = LinearVelocity;

	/* work out the derivative of R(t) and copy the result into the state array*/
	Temp0 = bnu::column(m_Rotate, 0);
	Cross(m_AngularMomentum, Temp0, Temp1);
	*it++ = Temp1;

	Temp0 = bnu::column(m_Rotate, 1);
	Cross(m_AngularMomentum, Temp0, Temp1);
	*it++ = Temp1;

	Temp0 = bnu::column(m_Rotate, 2);
	Cross(m_AngularMomentum, Temp0, Temp1);
	*it++ = Temp1;

	/* copy force and torque into the array */
	*it++ = m_Force;
	*it++ = m_Torque;

}
