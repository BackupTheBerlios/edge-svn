#pragma once
#include "../Math/DEStateSource.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace bnu = boost::numeric::ublas;

/*
For more information on simulating rigid bodies see http://www.pixar.com/companyinfo/research/pbm2001/.
*/

namespace Edge
{
	class RigidBody :
		public DEStateSource
	{
	public:
		RigidBody();
		virtual void GetState(StateType& State);
		virtual void GetStateDerivative(StateType& StateDerivative);

	private:
		/* State variables */
		bnu::vector<double> m_Position;
		bnu::matrix<double> m_Rotate;				
		bnu::vector<double> m_AngularMomentum;
		bnu::vector<double> m_LinearMomentum;
       
		/* Values that should remain constant during the simulation. 
		They are precalculated. */
		double m_Mass;
		bnu::matrix<double> m_BodySpaceInertiaTensor;
		bnu::matrix<double> m_BodySpaceInertiaTensorInv;
		
		//count of the number of vectors that makeup the state. 1 vector for position,
		//3 for rotate, 1 for angular momentum and 1 for linear momentum
		const int m_CountStateVectors; 
	};
	typedef boost::shared_ptr<RigidBody> RigidBodyPtr;
}