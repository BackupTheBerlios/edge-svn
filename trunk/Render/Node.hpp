#pragma once

#include <string>
#include <list>
#include <boost/enable_shared_from_this.hpp>

namespace Edge
{
	//forward declare the class NodeVisitor
	class NodeVisitor;
    class Node;
	typedef boost::shared_ptr<Node> NodePtr;
	class Node : 
		public boost::enable_shared_from_this<Node>
	{
	public:

		Node(void);
		virtual ~Node(void);
		void SetName(const std::string& Name) {m_Name = Name;}
		void SetParent(NodePtr pNode);
		const std::string& GetName() {return m_Name;}
		virtual void ApplyVisitor(Edge::NodeVisitor* pVisitor);	
		void AddChild(NodePtr pChild);
		void RemoveChild(NodePtr pChild);
		void SetNumChildren(int n);		
	
	private:
		std::string m_Name;	
		std::list<NodePtr> m_Children;
		Edge::NodePtr m_pParent;
	};	
	
}

