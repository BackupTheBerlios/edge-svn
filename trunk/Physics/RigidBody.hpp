#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace bnu = boost::numeric::ublas;

/*
For more information on simulating rigid bodies see http://www.pixar.com/companyinfo/research/pbm2001/.
*/

namespace Edge
{
	class RigidBody
	{
	public:		

	private:
		/* State variables */
		bnu::vector<double> m_Position;
		bnu::matrix<double> m_Rotate;				
		bnu::vector<double> m_AngularMomentum;
		bnu::vector<double> m_LinearMomentum;

		/* Derived quantities (i.e. State'). */
		bnu::vector<double> m_LinearVelocity;
		double m_AngularVelocity;
		bnu::matrix<double> m_InertiaTensor;
        
		/* Values that should remain constant during the simulation. 
		They are precalculated. */
		double m_Mass;
		bnu::matrix<double> m_BodySpaceInertiaTensor;
		bnu::matrix<double> m_BodySpaceInertiaTensorInv;
		
	};
	typedef boost::shared_ptr<RigidBody> RigidBodyPtr;
}