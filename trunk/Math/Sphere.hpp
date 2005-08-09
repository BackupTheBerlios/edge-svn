#pragma once

#include <boost/numeric/ublas/vector.hpp>

namespace bnu=boost::numeric::ublas;

namespace Edge
{
	class Sphere
	{
	public:
		Sphere(void);
		virtual ~Sphere(void);
		
		void SetCenter(const bnu::vector<double>& Center);
		void SetRadius(const double& Radius);
		
		double GetRadius() const;
		const bnu::vector<double>& GetCenter() const;

	private:
		bnu::vector<double> m_Center;
		double m_Radius;
	};
}
