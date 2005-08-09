#pragma once

#include "../../Render/Render.hpp"
#include "./PendulumNode.hpp"

class PendulumRenderer :
	public Edge::RenderVisitor
{
public:
	PendulumRenderer(void);
	virtual ~PendulumRenderer(void);

	void DrawPendulum(PendulumNode::PendulumNodePtr pPen) const;
};
