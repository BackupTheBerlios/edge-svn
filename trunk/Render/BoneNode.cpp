#include "./Bonenode.hpp"
#include "./NodeVisitor.hpp"

using namespace Edge;

BoneNode::BoneNode(void)
{
}

BoneNode::~BoneNode(void)
{
}

void 
BoneNode::SetMesh(Edge::MeshPtr pMesh)
{
	m_pMesh = pMesh;
}

void 
BoneNode::SetTranslate(Edge::TranslatePtr pTranslate)
{
	m_pTranslate = pTranslate;
}
		
void
BoneNode::ApplyVisitor(Edge::NodeVisitor* pVisitor)
{
	m_pTranslate->ApplyVisitor(pVisitor);
	m_pMesh->ApplyVisitor(pVisitor);
	Node::ApplyVisitor(pVisitor);
}
