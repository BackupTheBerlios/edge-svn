#pragma once

#include "../../Render/Render.hpp"

class SoftBodyNode :
	public Edge::DiffSystemNode
{
public:
	typedef boost::shared_ptr<SoftBodyNode> SoftBodyNodePtr;

	void Setup();
	SoftBodyNode(void);
	virtual ~SoftBodyNode(void);
};
