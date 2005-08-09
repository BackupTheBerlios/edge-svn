#pragma once

#include "./RGBColor.hpp"
#include "./Node.hpp"
#include "./Light.hpp"

namespace Edge
{
	class Light :
		public Node
	{
	public:
		typedef boost::shared_ptr<Light> LightPtr;

		Light(int ID = 0);
		virtual ~Light(void);

		const RGBColor& GetDiffuse() const {return m_Diffuse;}
		const RGBColor& GetAmbient() const {return m_Ambient;}

		void SetDiffuse(const RGBColor& Color) { m_Diffuse = Color; }
		void SetAmbient(const RGBColor& Color) { m_Ambient = Color; }
		int GetID() const { return m_ID; }
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor)=0;

	private:
		RGBColor m_Diffuse;
		RGBColor m_Ambient;	
		int m_ID;		
	};
}
