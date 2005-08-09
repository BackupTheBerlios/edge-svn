#pragma once
#include "./Insect.hpp"
#include "../../Render/RenderVisitor.hpp"

class InsectRenderer :
	public Edge::RenderVisitor
{
public:
	InsectRenderer(void);
	virtual ~InsectRenderer(void);

	void DrawInsect(Insect::InsectPtr) const;
};
