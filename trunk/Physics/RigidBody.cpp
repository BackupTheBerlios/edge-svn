#include "./RigidBody.hpp"
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace Edge;

RigidBody::RigidBody() :
	m_Position(3),
	m_CountStateVectors(6)
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
	bnu::matrix<double> DRotate;
	bnu::vector<double> AngularMomentum;
	if (State.size() != m_CountStateVectors)
	{
		State.resize(m_CountStateVectors);
	}
	bnu::vector<double> LinearVelocity(m_LinearMomentum/m_Mass);
	bnu::matrix<double> InertiaTensor((bnu::prod(m_Rotate, m_BodySpaceInertiaTensor)));
	InertiaTensor = bnu::prod(InertiaTensor, bnu::trans(m_Rotate));
	*it++ = LinearVelocity;

}
