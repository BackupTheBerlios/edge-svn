#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include "./RenderVisitor.hpp"
#include "../Math/MatrixSupport.hpp"

using namespace Edge;
using std::map;

RenderVisitor::RenderVisitor(void)
{
	
}

RenderVisitor::~RenderVisitor(void)
{
}

void RenderVisitor::Visit(TranslatePtr pTrans)
{
	double x, y, z;
	pTrans->GetTranslate(x, y, z);
	glPushMatrix();
	glTranslatef((GLfloat)x,(GLfloat)y, (GLfloat)z);
}

void RenderVisitor::Leave(TranslatePtr pTrans)
{
	glPopMatrix();
}

void RenderVisitor::Visit(MeshPtr pM)
{
	Mesh::TDConnectContainer::const_iterator it;	
	glBegin(GL_TRIANGLES);
	const Mesh::TDConnectContainer& Connect = pM->GetConnect();
	const Mesh::TDVertexContainer& Vertices = pM->GetVertices();
	const Mesh::NormalArrayType& Normals = pM->GetNormals();
	const Mesh::TexCoordArrayType& TexCoord = pM->GetTexCoords();
	for (it = Connect.begin(); it != Connect.end(); ++it)
	{
		int nIdx0 = (*it)[0];
		int nIdx1 = (*it)[1];
		int nIdx2 = (*it)[2];

		double x = Vertices[nIdx0][0];//.x;
		double y = Vertices[nIdx0][1];//.y;
		double z = Vertices[nIdx0][2];//.z;
		double nx = Normals[nIdx0][0];//.x;
		double ny = Normals[nIdx0][1];//.y;
		double nz = Normals[nIdx0][2];//.z;
		double u, v;
		if (pM->GoodTexCoords())
		{
			u = TexCoord[nIdx0][0];
			v = TexCoord[nIdx0][1];
			glTexCoord2d(u, v);
		}
		glNormal3d(nx, ny, nz);
		glVertex3d(x, y, z);

		x = Vertices[nIdx1][0];//.x;
		y = Vertices[nIdx1][1];//.y;
		z = Vertices[nIdx1][2];//.z;
		nx = Normals[nIdx1][0];//.x;
		ny = Normals[nIdx1][1];//.y;
		nz = Normals[nIdx1][2];//.z;
		if (pM->GoodTexCoords())
		{
			u = TexCoord[nIdx1][0];
			v = TexCoord[nIdx1][1];
			glTexCoord2d(u, v);
		}
		glNormal3d(nx, ny, nz);
		glVertex3d(x, y, z);

		x = Vertices[nIdx2][0];//.x;
		y = Vertices[nIdx2][1];//.y;
		z = Vertices[nIdx2][2];//.z;
		nx = Normals[nIdx2][0];//.x;
		ny = Normals[nIdx2][1];//.y;
		nz = Normals[nIdx2][2];//.z;
		if (pM->GoodTexCoords())
		{
			u = TexCoord[nIdx2][0];
			v = TexCoord[nIdx2][1];
			glTexCoord2d(u, v);
		}
		glNormal3d(nx, ny, nz);
		glVertex3d(x, y, z);
	}
	glEnd();
}

void RenderVisitor::Leave(MeshPtr pMesh)
{

}

void RenderVisitor::Visit(MaterialPtr pMat)
{
	GLfloat rgb[3];
	rgb[0] = (GLfloat)pMat->GetDiffuse().m_Red;
	rgb[1] = (GLfloat)pMat->GetDiffuse().m_Green;
	rgb[2] = (GLfloat)pMat->GetDiffuse().m_Blue;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
	rgb[0] = (GLfloat)pMat->GetAmbient().m_Red;
	rgb[1] = (GLfloat)pMat->GetAmbient().m_Green;
	rgb[2] = (GLfloat)pMat->GetAmbient().m_Blue;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rgb);
}

void RenderVisitor::Leave(MaterialPtr pMat)
{

}

void RenderVisitor::Visit(BoundNodePtr pBNode)
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);	
	BoundingBoxPtr pBox(pBNode->GetBoundingBox());
	//front face
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MaxZ);

	//back face
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MinZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MinZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MinZ);
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MinZ);

	//left face
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MinZ);
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MinZ);

	//right face
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MinZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MinZ);

	//top face
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MinZ);
	glVertex3d(pBox->m_MinX, pBox->m_MaxY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MaxY, pBox->m_MinZ);

	//bottom face
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MinZ);
	glVertex3d(pBox->m_MinX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MaxZ);
	glVertex3d(pBox->m_MaxX, pBox->m_MinY, pBox->m_MinZ);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void RenderVisitor::Leave(BoundNodePtr pBox)
{

}

void RenderVisitor::Visit(DirectionLightPtr pL)
{	
	glEnable(GL_LIGHT0 + pL->GetID());
	//glMatrixMode(GL_MODELVIEW_MATRIX);
	//glPushMatrix();
	//glLoadIdentity();
	GLfloat Color[4];
	Color[0] = (GLfloat)pL->GetDiffuse().m_Red;
	Color[1] = (GLfloat)pL->GetDiffuse().m_Green;
	Color[2] = (GLfloat)pL->GetDiffuse().m_Blue;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_DIFFUSE, Color);
	Color[0] = (GLfloat)pL->GetAmbient().m_Red;
	Color[1] = (GLfloat)pL->GetAmbient().m_Green;
	Color[2] = (GLfloat)pL->GetAmbient().m_Blue;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_AMBIENT, Color);
	GLfloat Position[4];
	Position[0] = -(GLfloat)pL->GetDirection()[0];
	Position[1] = -(GLfloat)pL->GetDirection()[1];
	Position[2] = -(GLfloat)pL->GetDirection()[2];
	Position[3] = (GLfloat)0.0;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_POSITION, Position);	
	//glPopMatrix();
	//glLightfv(GL_LIGHT0 + pL->GetID(), GL_SPOT_DIRECTION, Position);	
}

void RenderVisitor::Leave(DirectionLightPtr pL)
{

}

void RenderVisitor::Visit(AmbientLightPtr pL)
{	
	glEnable(GL_LIGHT0 + pL->GetID());
	GLfloat Color[4];
	Color[0] = (GLfloat)pL->GetDiffuse().m_Red;
	Color[1] = (GLfloat)pL->GetDiffuse().m_Green;
	Color[2] = (GLfloat)pL->GetDiffuse().m_Blue;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_DIFFUSE, Color);
	Color[0] = (GLfloat)pL->GetAmbient().m_Red;
	Color[1] = (GLfloat)pL->GetAmbient().m_Green;
	Color[2] = (GLfloat)pL->GetAmbient().m_Blue;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_AMBIENT, Color);
}

void RenderVisitor::Leave(AmbientLightPtr pL)
{

}

void RenderVisitor::Visit(PointLightPtr pL)
{	
	glEnable(GL_LIGHT0 + pL->GetID());
	GLfloat Color[4];
	Color[0] = pL->GetDiffuse().m_Red;
	Color[1] = pL->GetDiffuse().m_Green;
	Color[2] = pL->GetDiffuse().m_Blue;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_DIFFUSE, Color);
	GLfloat Position[4];
	Position[0] = pL->GetPosition()[0];
	Position[1] = pL->GetPosition()[1];
	Position[2] = pL->GetPosition()[2];
	Position[3] = 1.0f;
	glLightfv(GL_LIGHT0 + pL->GetID(), GL_POSITION, Position);	
}

void RenderVisitor::Leave(PointLightPtr pL)
{

}

void RenderVisitor::Visit(RotatePtr pRot)
{
	glPushMatrix();
	const double Angle(RadiansToDegrees(pRot->GetAngle()));
	const bnu::vector<double> Axis(pRot->GetAxis());
	glRotatef(Angle, Axis[0], Axis[1], Axis[2]);
}

void RenderVisitor::Leave(RotatePtr pRot)
{
	glPopMatrix();
}

void RenderVisitor::Visit(ScalePtr pScale)
{
	glPushMatrix();
	const bnu::vector<double> S(pScale->GetScale());
	glScalef((GLfloat)S[0], (GLfloat)S[1], (GLfloat)S[2]);
}

void RenderVisitor::Leave(ScalePtr pScale)
{
	glPopMatrix();
}

void RenderVisitor::Visit(TextureMapPtr pT)
{
	map<TextureMapPtr, GLuint>::const_iterator itPair(m_TextureToID.find(pT));
	GLuint ID = itPair->second;
	TextureMap::ApplyMode AM(pT->GetApplyMode());
	if (AM == TextureMap::AM_MODULATE) 
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (AM == TextureMap::AM_REPLACE)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, ID);
	assert(GL_NO_ERROR == glGetError());
}

void RenderVisitor::Leave(TextureMapPtr pT)
{

}


