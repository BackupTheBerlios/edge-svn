#pragma once


#include "./DEStateSource.hpp"

namespace bnu = boost::numeric::ublas;

namespace Edge
{
	/**
	Models y'' + 3y' + 2y = 0, y(0) = 1, y'(0) = 1 (which hopefully explains
	the odd looking class name)
	*/
	class DDy_3Dy_2y :
		public DEStateSource
	{
	private:
		bnu::vector<double> x0;
		bnu::vector<double> x1;

	public:
		DDy_3Dy_2y(void);
		virtual ~DDy_3Dy_2y(void);

		virtual void GetState(StateType& States);
		virtual void GetStateDerivative(StateType& StateDerivatives);
		virtual void SetState(const StateType& State);	

	};
}
