#pragma once

#include "./PartConstRB.hpp"

namespace Edge
{
	class Chain :		
		public PartConstRB
	{
	public:
		typedef boost::shared_ptr<Chain> ChainPtr;
		
		Chain();
		virtual ~Chain(void);						
		virtual void BuildSystem();	
		void AddForce(int Idx, double x, double y, double z);
		virtual void GetStateDerivative(StateType& StateDerivative);
	};
}
