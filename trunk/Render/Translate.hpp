#pragma once

#include "./Node.hpp"
#include <boost/numeric/ublas/vector.hpp>

namespace bnu=boost::numeric::ublas;

namespace Edge
{	
	class Translate :
		public Edge::Node		
	{	
	public:
		Translate(void);
		virtual ~Translate(void);				
		void SetTranslate(double x, double y, double z);
		void SetTranslate(bnu::vector<double> Trans);
		void GetTranslate(double& x, double& y, double& z) const;
		void GetTranslate(bnu::vector<double>& Trans) { Trans = m_Translate; }
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);				

	private:
		bnu::vector<double> m_Translate;
	};

	typedef boost::shared_ptr<Translate> TranslatePtr;
}
