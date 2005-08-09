#pragma once
#include "./Light.hpp"

namespace Edge
{
	class AmbientLight :
		public Edge::Light
	{
	public:

		AmbientLight(int ID=0);
		virtual ~AmbientLight(void);
		virtual void ApplyVisitor(NodeVisitor* pVisitor);
	};
	typedef boost::shared_ptr<AmbientLight> AmbientLightPtr;
}
