#pragma once
#include <boost/numeric/ublas/vector.hpp>
#include "./Node.hpp"

namespace bnu = boost::numeric::ublas;

namespace Edge
{
	class Rotate :
		public Node		
	{
	public:
		Rotate(void);
		virtual ~Rotate(void);
		
		void SetAngle(double a) {m_Angle = a;}
		void SetAxis(double, double, double);
		void SetAngleAxis(double, const bnu::vector<double>&);
		double GetAngle() const {return m_Angle;}
		const bnu::vector<double>& GetAxis() const {return m_Axis;}		
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);

	private:
		bnu::vector<double> m_Axis;
		double m_Angle;
	};
	typedef boost::shared_ptr<Rotate> RotatePtr;
}
