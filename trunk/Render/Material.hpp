#pragma once

#include "./Node.hpp"
#include "./RGBColor.hpp"

namespace Edge
{
	class Material :
		public Node		
	{
	public:

		Material(void);
		virtual ~Material(void);

		void SetDiffuse(const RGBColor& Col);
		void SetAmbient(const RGBColor& Col);
		const RGBColor& GetAmbient() const;
		const RGBColor& GetDiffuse() const;		
		void ApplyVisitor(Edge::NodeVisitor* pVisitor);

	private:
		RGBColor m_Diffuse;
		RGBColor m_Ambient;
	};
	typedef boost::shared_ptr<Material> MaterialPtr;
}
