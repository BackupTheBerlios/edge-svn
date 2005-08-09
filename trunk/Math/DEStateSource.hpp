#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace Edge
{
	/**
	A class that represents any differentiable system, be it y(t) = y'(t) 
	or a mass spring system. This classes that inherit from this class
	are used in conjunction with a differential equation solver, like EulerSolver
	*/
	class DEStateSource
	{	
	public:
		typedef boost::shared_ptr<DEStateSource> DEStateSourcePtr;
		typedef std::vector < boost::numeric::ublas::vector<double> > StateType;		

		DEStateSource();
		virtual ~DEStateSource(void);
		
		/**
		@param State The current state of the system.
		*/
		virtual void GetState(StateType& State){};

		/**
		@param StateDerivative The state derivative of the system.
		*/
		virtual void GetStateDerivative(StateType& StateDerivative){};

		/**
		@param State Set the state of the system
		*/
		virtual void SetState(const StateType& State){};

		/**
		@param State Increment the system time by dt. Note that incrementing the time of
		the system isn't sufficient to change the state. The state of the system will
		change when used with one of the solver classes (i.e. RK4Solver)
		*/
		void IncTime(double dt);
		double GetTime() const;
		void SetTime(double t);

	private:
		double m_Time;
	};
}
