#pragma once
#include "./Node.hpp"
#include "./Translate.hpp"
#include "./NodeVisitor.hpp"
#include "../Math/Math.hpp"

namespace Edge
{
	/**
	A Node that contains a differentiable system. So whenever this time steps (via 
	a visitor)
	*/
	class DiffSystemNode :
		public Node
	{
	public:
		typedef boost::shared_ptr<DiffSystemNode> DiffSystemNodePtr;
		
		DiffSystemNode(void);
		virtual ~DiffSystemNode(void);
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);		
		Edge::DEStateSource::DEStateSourcePtr GetDEStateSource() {return m_pStateSource;}
		void SetDEStateSource(Edge::DEStateSource::DEStateSourcePtr pDS) {m_pStateSource = pDS;}		
		
	private:		
		Edge::DEStateSource::DEStateSourcePtr m_pStateSource;		
	};
}
