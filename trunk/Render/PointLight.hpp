#pragma once
#include <boost/numeric/ublas/vector.hpp>
#include "./Light.hpp"

namespace bnu = boost::numeric::ublas;


namespace Edge
{
	class PointLight :
		public Light		
	{
	public:
		
		PointLight(int ID = 0);
		virtual ~PointLight(void);

		void SetPosition(double x, double y, double z);
		const bnu::vector<double>& GetPosition() const;	
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);

	private:
		bnu::vector<double> m_Position;
	};
	typedef boost::shared_ptr<PointLight> PointLightPtr;
}
