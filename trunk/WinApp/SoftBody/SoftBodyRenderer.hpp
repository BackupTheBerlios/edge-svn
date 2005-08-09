#pragma once

#include "../../Render/Render.hpp"
#include "SoftBodyNode.hpp"

class SoftBodyRenderer :
	public Edge::RenderVisitor
{
public:
	SoftBodyRenderer(void);
	virtual ~SoftBodyRenderer(void);

	void Draw(SoftBodyNode::SoftBodyNodePtr pSB) const;
};
