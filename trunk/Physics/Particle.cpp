//#include "StdAfx.h"
#include "./Particle.hpp"
#include <boost/numeric/ublas/vector.hpp>

using namespace Edge;
using namespace boost::numeric;
using namespace std;

Particle::Particle(void) :
	m_Velocity(3),
	m_Position(3),
	m_Mass(1)
{
}

Particle::Particle(const ublas::vector<double>& Position, 
		 const ublas::vector<double>& Velocity, double Mass) :
	m_Position(Position),
	m_Velocity(Velocity),
	m_Mass(Mass)
{

}

Particle::~Particle(void)
{

}

double Particle::GetKineticEnergy() const
{
	return 0.5*m_Mass*inner_prod(m_Velocity, m_Velocity);
}

ostream& Edge::operator << (ostream& Out, const Particle& P)
{
	Out << /*P.m_Mass << "," <<*/ P.m_Position[0] /*<< " " << P.m_Position[1] << " " << P.m_Position[2] <<","
		<< P.m_Velocity[0] << " " << P.m_Velocity[1] << " " << P.m_Velocity[2]*/;
	return Out;
}

void Particle::SetPosition(double x, double y, double z)
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;
}

void Particle::SetVelocity(double x, double y, double z)
{
	m_Velocity[0] = x;
	m_Velocity[1] = y;
	m_Velocity[2] = z;
}
