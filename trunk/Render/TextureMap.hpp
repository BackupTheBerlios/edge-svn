#pragma once

#include <GL/glaux.h>
#include "./Node.hpp"

namespace Edge
{
	class TextureMap :
		public Node		
	{
	public:
		enum ApplyMode
		{
			AM_REPLACE,
			AM_MODULATE
		};

		TextureMap(void);
		virtual ~TextureMap(void);
		
		bool LoadBMP(const std::string& Filename);		
		int GetWidth() const { return m_pBitmap->sizeX; }
		int GetHeight() const { return m_pBitmap->sizeY; }
		const unsigned char* GetDataPtr() const { return m_pBitmap->data; }
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);
		void SetApplyMode(Edge::TextureMap::ApplyMode AM);
		ApplyMode GetApplyMode() { return m_AM; }

	private:
		std::string m_Filename;
		AUX_RGBImageRec* m_pBitmap;		
		ApplyMode m_AM;
	};

	typedef boost::shared_ptr<TextureMap> TextureMapPtr;
}
