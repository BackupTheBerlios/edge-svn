#pragma once

#include <boost/numeric/ublas/vector.hpp>

namespace bnu=boost::numeric::ublas;

namespace Edge
{
	class Ray
	{
	public:
		Ray(void);
		virtual ~Ray(void);
		void SetOrigin(const bnu::vector<double>& Origin);
		void SetDirection(const bnu::vector<double>& Dir);

		const bnu::vector<double>& GetDirection() const;
		const bnu::vector<double>& GetOrigin() const;

	private:
		bnu::vector<double> m_Origin;
		bnu::vector<double> m_Direction;
	};
}