#pragma once
#include <boost/shared_ptr.hpp>
namespace Edge
{
	class Spring
	{
	public:
		typedef boost::shared_ptr<Spring> SpringPtr;

		Spring(void);
		~Spring(void);
		double GetConstant() { return m_Constant; }
		void SetConstant(double Constant) { m_Constant = Constant; }
		double GetRestLen() { return m_RestLen; }
		void SetRestLen(double RestLen) { m_RestLen = RestLen; }

	private:
		double m_Constant;	
		double m_RestLen;
	};
}
