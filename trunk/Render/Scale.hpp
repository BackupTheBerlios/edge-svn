#pragma once
#include <boost/numeric/ublas/vector.hpp>
#include "./Node.hpp"

namespace bnu = boost::numeric::ublas;

namespace Edge
{
	class Scale :
		public Node		
	{
	public:
		Scale(void);
		~Scale(void);
		
		const bnu::vector<double>& GetScale() const { return m_Scale; }
		void GetScale(double& x, double& y, double& z) const;
		void SetScale(double x, double y, double z);
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);
		
	private:
		bnu::vector<double> m_Scale;
	};

	typedef boost::shared_ptr<Scale> ScalePtr;
}
