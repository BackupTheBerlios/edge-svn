#include <boost/numeric/ublas/vector.hpp>
#include "./CircularConstraint.hpp"

using namespace Edge;
using DEStateSource::StateType;
using namespace boost::numeric;

CircularConstraint::CircularConstraint() :
	m_Force(3)
{
	m_Force[0] = 0; m_Force[1] = 0; m_Force[2] = 0;
}

CircularConstraint::~CircularConstraint(void)
{
	
}

CircularConstraint::CircularConstraint(double x, double y, double z, double dx, double dy, double dz) :
	m_Force(3)
{
	m_Force[0] = 0; m_Force[1] = 0; m_Force[2] = 0;
	m_Particle.SetPosition(x,y,z);
	m_Particle.SetVelocity(dx,dy,dz);
}

ublas::vector<double> CircularConstraint::GetConstraintForce()const
{		
	double Mass = m_Particle.GetMass();
	ublas::vector<double> x(m_Particle.GetPosition());
	ublas::vector<double> dx(m_Particle.GetVelocity());
	double a = ublas::inner_prod(-m_Force,x);
	double b = Mass*ublas::inner_prod(dx,dx);
	double c = ublas::inner_prod(x,x);
	double lambda = (a - b)/c;
	return lambda*x;
}

void CircularConstraint::GetState(StateType& State) const
{
	if (State.size() != 2)
		State.resize(2);

	State[0] = m_Particle.GetPosition();
	State[1] = m_Particle.GetVelocity();
}

void CircularConstraint::SetState(const StateType& State) 
{
	assert(State.size() == 2);
	m_Particle.SetPosition(State[0]);
	m_Particle.SetVelocity(State[1]);
}

void CircularConstraint::GetStateDerivative(DEStateSource::StateType& StateDerivative) const
{
	if (StateDerivative.size() != 2)
		StateDerivative.resize(2);	
	ublas::vector<double> FHat = GetConstraintForce();
	StateDerivative[0] = m_Particle.GetVelocity();
	StateDerivative[1] = (m_Force+FHat)/m_Particle.GetMass();
}
