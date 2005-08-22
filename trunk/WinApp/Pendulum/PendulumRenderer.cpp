#include "./PendulumRenderer.hpp"

using std::vector;
using namespace Edge;

PendulumRenderer::PendulumRenderer(void)
{
}

PendulumRenderer::~PendulumRenderer(void)
{
}

void PendulumRenderer::DrawPendulum(PendulumNode::PendulumNodePtr pPen) const
{
	DEStateSource::DEStateSourcePtr pDEState(pPen->GetDEStateSource());
	DEStateSource::StateType State;
	pDEState->GetState(State);
	DEStateSource::StateType::iterator it(State.begin());
	//assert(State.size() >= 4);	
	const vector<int>& Connect(pPen->GetConnect());
	vector<int>::size_type n = Connect.size();
	for (vector<int>::size_type i = 0; i < n; i+=2)
	{
		bnu::vector<double>& Pos(State[Connect[i]*2]);
		bnu::vector<double>& Pos1(State[Connect[i+1]*2]);
		float rgb[3];
		rgb[0] = 0.0;
		rgb[1] = 0.0;
		rgb[2] = 0.7;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rgb);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
		glBegin(GL_LINES);
		glVertex3f(Pos[0],Pos[1], Pos[2]);
		glVertex3f(Pos1[0],Pos1[1], Pos1[2]);
		glEnd();
	}
		
}