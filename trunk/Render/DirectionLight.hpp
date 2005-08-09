#pragma once
#include <boost/numeric/ublas/vector.hpp>
#include "./Light.hpp"

namespace bnu = boost::numeric::ublas;

namespace Edge
{	
	class DirectionLight :
		public Light		
	{
	public:

		DirectionLight(int ID = 0);
		virtual ~DirectionLight(void);

		const bnu::vector<double>& GetDirection() const {return m_Direction;}
		void SetDirection(bnu::vector<double>& Direction) {m_Direction = Direction;}
		void SetDirection(double x, double y, double z);	
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);

	private:
		bnu::vector<double> m_Direction;
	};
	typedef boost::shared_ptr<DirectionLight> DirectionLightPtr;
}
