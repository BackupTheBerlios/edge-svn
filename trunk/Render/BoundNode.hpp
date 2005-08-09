#pragma once

#include "./BoundingBox.hpp"
#include "./Node.hpp"
#include <boost/shared_ptr.hpp>
namespace Edge
{
	class BoundNode :
		public Edge::Node
	{
	public:

		BoundNode(void);
		virtual ~BoundNode(void);
		Edge::BoundingBoxPtr GetBoundingBox();
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
		
	private:
		Edge::BoundingBoxPtr m_pBox;
	};
	typedef boost::shared_ptr<BoundNode> BoundNodePtr;
}
