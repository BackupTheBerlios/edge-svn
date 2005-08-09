#pragma once
#include "./DEStateSource.hpp"

namespace bnu=boost::numeric::ublas;

namespace Edge
{
	/**
	Represents the differential equation y'(t) = y(t), y(0) = 1.  
	The solution to this is e (the exponential function). Therefore 
	if you run a solver on this system from t=0 to t=1, 
	you should get an approximation to e. The accuracy of the solution depends on the
	time step given to one of the differential equation solver classes. 
	@see RK4Solver
	*/

	class ExpDE :
		public DEStateSource
	{
	private:
		bnu::vector<double> x;

	public:
		ExpDE();
		virtual ~ExpDE();

		/**			
		Get the state of the system. In the case of the exponential function the state is a single number
		representing an approximation of e^t.		
		*/
		virtual void GetState(Edge::DEStateSource::StateType& State);
		
		/**			
		In the case of the exponential function the state derivative is equal to the state.		
		*/
		virtual void GetStateDerivative(Edge::DEStateSource::StateType& StateDerivative);

		virtual void SetState(const Edge::DEStateSource::StateType& State);	
	};
}
