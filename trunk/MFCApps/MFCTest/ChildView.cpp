// ChildView.cpp : implementation of the ChildView class
//

#include "stdafx.h"
#include "resource.h"
#include "./MFCTest.h"
#include "../../Render/OpenGLRenderer.hpp"
#include "../../Render/Light.hpp"
#include "../../Math/RK4Solver.hpp"
#include "../../Render/Camera.hpp"
#include ".\childview.h"

using namespace std;
using namespace boost::numeric;
using namespace Edge;
using Mesh::MeshPtr;
using Particle::ParticlePtr;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*A lot of this code is from the mfc cube sample in MSDN that shows how to integrate OpenGL
with MFC. The code is administrative trivia that's not worth understanding.*/

unsigned char threeto8[8] =
{
	0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] =
{
	0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] =
{
	0, 255
};

static int defaultOverride[13] =
{
	0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] =
{
	{ 0,   0,   0,    0 },
	{ 0x80,0,   0,    0 },
	{ 0,   0x80,0,    0 },
	{ 0x80,0x80,0,    0 },
	{ 0,   0,   0x80, 0 },
	{ 0x80,0,   0x80, 0 },
	{ 0,   0x80,0x80, 0 },
	{ 0xC0,0xC0,0xC0, 0 },

	{ 192, 220, 192,  0 },
	{ 166, 202, 240,  0 },
	{ 255, 251, 240,  0 },
	{ 160, 160, 164,  0 },

	{ 0x80,0x80,0x80, 0 },
	{ 0xFF,0,   0,    0 },
	{ 0,   0xFF,0,    0 },
	{ 0xFF,0xFF,0,    0 },
	{ 0,   0,   0xFF, 0 },
	{ 0xFF,0,   0xFF, 0 },
	{ 0,   0xFF,0xFF, 0 },
	{ 0xFF,0xFF,0xFF, 0 }
};

/////////////////////////////////////////////////////////////////////////////
// ChildView

//IMPLEMENT_DYNCREATE(ChildView, CView)

BEGIN_MESSAGE_MAP(ChildView, CView)
	//{{AFX_MSG_MAP(ChildView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
//	ON_WM_TIMER()
//	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_COMMAND(ID_ACTIONS_STEP, OnActionsStep)
	ON_COMMAND(ID_ACTIONS_PLAY, OnActionsPlay)
	ON_COMMAND(ID_ACTIONS_STOP, OnActionsStop)
	ON_COMMAND(ID_ACTIONS_WRITETOFILE, OnActionsWriteToFile)
	ON_COMMAND(ID_CREATE_1DGRID, OnCreate1DGrid)
	ON_COMMAND(ID_CREATE_2DGRID, OnCreate2DGrid)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChildView construction/destruction

ChildView::ChildView() :
	m_NearPlane(1.0f),
	m_FarPlane(100.0f),
	m_TimeStep(0.1),
	m_CurrentTime(0),
	m_WriteToFile(false),
//	m_pGrid1D(new Grid1D(6,3)),
//	m_pGrid2D(new Grid2D(3,3,3))
	m_pMS(NULL)
{
	m_pDC = NULL;
	m_pOldPalette = NULL;
	m_play = FALSE;
	/*EyeX = 0;
	EyeY = 0;
	EyeZ = 10;	*/
	ublas::vector<double> Pos(3);
	ublas::vector<double> Left(3);
	ublas::vector<double> Up(3);
	ublas::vector<double> Dir(3);
	Pos[0] = 0; Pos[1] = 0; Pos[2] = 10;
	m_Camera.SetPosition(Pos);
	Left[0] = -1; Left[1] = 0; Left[2] = 0;
	Up[0] = 0; Up[1] = 1; Up[2] = 0;
	Dir[0] = 0; Dir[1] = 0; Dir[2] = -1;
	m_Camera.SetAxes(Left, Up, Dir);
}

ChildView::~ChildView()
{
	if (m_pMS) delete m_pMS;
}

/////////////////////////////////////////////////////////////////////////////
// ChildView drawing

void ChildView::OnDraw(CDC* /*pDC*/)
{

	DrawScene();
}

/////////////////////////////////////////////////////////////////////////////
// ChildView diagnostics

#ifdef _DEBUG
void ChildView::AssertValid() const
{
	CView::AssertValid();
}

void ChildView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ChildView message handlers

BOOL ChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style. Refer to SetPixelFormat
	// documentation in the "Comments" section for further information.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

int ChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init(); // initialize OpenGL
	return 0;
}

void ChildView::OnDestroy()
{
	HGLRC   hrc;

	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;

	CView::OnDestroy();
}

void ChildView::OnSize(UINT nType, int cx, int cy)
{
	//TODO: Fix OnSize events so that they dont destroy the rendering when using gluLookAt	
	CView::OnSize(nType, cx, cy);

	if(cy > 0)
	{
		glViewport(0, 0, cx, cy);

		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx/cy, m_NearPlane, m_FarPlane);
		glMatrixMode(GL_MODELVIEW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// GL helper functions

void ChildView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	CreateRGBPalette();

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, m_oldRect.right, m_oldRect.bottom);
	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 1.0f;
	//fFarPlane = 7.0f;
	fFarPlane = 100.0f;
	//fMaxObjSize = 3.0f;
	//m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, m_NearPlane, m_FarPlane);
	glMatrixMode(GL_MODELVIEW);
	m_OpenGLRenderingContext = hrc;	
}

BOOL ChildView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}

unsigned char ChildView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
	unsigned char val;

	val = (unsigned char) (i >> shift);
	switch (nbits)
	{

	case 1:
		val &= 0x1;
		return oneto8[val];
	case 2:
		val &= 0x3;
		return twoto8[val];
	case 3:
		val &= 0x7;
		return threeto8[val];

	default:
		return 0;
	}
}

#pragma warning(disable : 4244)
void ChildView::CreateRGBPalette()
{
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int n, i;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE)
	{
		n = 1 << pfd.cColorBits;
		pPal = (PLOGPALETTE) new char[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

		ASSERT(pPal != NULL);

		pPal->palVersion = 0x300;
		pPal->palNumEntries = n;
		for (i=0; i<n; i++)
		{
			pPal->palPalEntry[i].peRed =
					ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
			pPal->palPalEntry[i].peGreen =
					ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
			pPal->palPalEntry[i].peBlue =
					ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
			pPal->palPalEntry[i].peFlags = 0;
		}

		/* fix up the palette to include the default GDI palette */
		if ((pfd.cColorBits == 8)                           &&
			(pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
			(pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
			(pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
		   )
		{
			for (i = 1 ; i <= 12 ; i++)
				pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
		}

		m_cPalette.CreatePalette(pPal);
		delete [] pPal;

		m_pOldPalette = m_pDC->SelectPalette(&m_cPalette, FALSE);
		m_pDC->RealizePalette();
	}
}
#pragma warning(default : 4244)

void ChildView::DrawScene(void)
{
	static BOOL     bBusy = FALSE;
	
	if(bBusy)
		return;
	bBusy = TRUE;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	RECT rect;
	GetClientRect(&rect);
	gluPerspective(45.0f, (GLdouble)rect.right/rect.bottom, m_NearPlane, m_FarPlane);
	//gluLookAt(EyeX, EyeY, EyeZ, EyeX, EyeY, EyeZ-1, 0, 1, 0);
	ublas::vector<double> LookAt(3);
	LookAt = m_Camera.GetPostion() + 10.0*m_Camera.GetDir();
	gluLookAt(m_Camera.GetPostion()[0], m_Camera.GetPostion()[1], m_Camera.GetPostion()[2], 
		LookAt[0], LookAt[1], LookAt[2], 
		m_Camera.GetUp()[0], m_Camera.GetUp()[1], m_Camera.GetUp()[2]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	{
		Light L;
		L.SetAmbient(RGBColor(0.5, 0.5, 0.5));
		m_OGL.SetBackgroundColor(0, 0, 0);
		m_OGL.SetLight(&L);		
		//Mesh Sphere;		
		//Sphere.LoadObj("../../Models/sphere.obj");
		for (int i = 0; i < m_Particles.size(); ++i)
		{
			m_OGL.Draw(*(m_Particles[i]));			
		}
		//m_OGL.Draw(Sphere);
	}
	glPopMatrix();
	glFinish();
	SwapBuffers(wglGetCurrentDC());
	bBusy = FALSE;	
}

void ChildView::PostNcDestroy()
{
	//CView::PostNcDestroy();
}

BOOL ChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}

void ChildView::OnTimer(UINT nIDEvent)
{		
	RK4Solver RK4;	
	m_CurrentTime += m_TimeStep;
	RK4.StepState(*m_pMS, m_TimeStep);	
	//write positions to file
	if (m_WriteToFile)
	{
		m_File << *m_pMS << ",";
		WriteAnalytical();
		m_File << "\n";
	}
	UpdateParticles();
	DrawScene();
}

void ChildView::UpdateParticles()
{
	//update the meshes with the new position information
	MassSpringSystem::ParticleIterator It, ItEnd;
	It = m_pMS->ParticleBegin();
	ItEnd = m_pMS->ParticleEnd();
	vector<Mesh::MeshPtr>::iterator ItMesh;
	ItMesh = m_Particles.begin();
	while (It != ItEnd)
	{
		ParticlePtr pP(*It);
		Mesh::MeshPtr pMesh(*ItMesh);
		pMesh->SetWorldTranslate(pP->GetLocation());
		++It;
		++ItMesh;
	}
}

void ChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'w':
		//EyeZ -= 0.5;
		m_Camera.MoveForward();
		break;

	case 's':
		//EyeZ += 0.5;
		m_Camera.MoveBackward();
		break;

	case 'a':
//		EyeX -= 0.5;
		m_Camera.StrafeLeft();
		break;
	
	case 'd':
//		EyeX += 0.5;
		m_Camera.StrafeRight();
		break;
	}
	DrawScene();
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void ChildView::OnActionsStep()
{	
	RK4Solver RK4;
	m_CurrentTime += m_TimeStep;
	m_pMS->IncTime(m_TimeStep);
	RK4.StepState(*m_pMS, m_TimeStep);	
	DrawScene();
}

void ChildView::OnActionsPlay()
{
//	m_pMS.TranslateParticle(1, Trans);	
	SetTimer(1, 100, NULL);
}

void ChildView::OnActionsStop()
{
	KillTimer(1);
}

void ChildView::OnActionsWriteToFile()
{		
	m_WriteToFile = !m_WriteToFile;
	stringstream FileName;
	FileName << "MassSpringSystem.txt";
	m_File.open(FileName.str().c_str(), ios_base::out);
	/*m_File << "Note - for the analytical solution assume that: k_0 = 1, k_1 = 1, m = 1.5, x(0) = -1, x'(0) = 0" << "\n";
	m_File << "x(t) = x(0)*cos(sqrt(K)*t), where K = (k_0+k_1)/m and T = (2*pi)/sqrt(K)";
	m_File << "The analytical results are given on the line below the numerical results at every time step\n";
	m_File << "Numerical: Time,Damping,Particles{{Mass,Postion,Velocity}....},KE,PE,KE+PE\n";	
	m_File << "Analytical: Time,Postion,x(t),Period\n";	*/
}

void ChildView::OnCreate1DGrid()
{
	// TODO: Add your command handler code here
	m_CurrentTime = 0;
	if (m_pMS) delete m_pMS;
	m_pMS = new Grid1D(3,3);
	m_pMS->SetDamping(0.0);
	int NumParticles = m_pMS->GetNumParticles();	
	m_Particles.resize(NumParticles);	
	MeshPtr pSphere(new Mesh);
	pSphere->LoadObj("sphere.obj");
	for (int i = 0; i < NumParticles; ++i)
	{	
		MeshPtr pMesh(new Mesh(*pSphere));
		m_Particles[i] = pMesh;
	}	
	
	MassSpringSystem::ParticleIterator It, ItEnd;
	It = m_pMS->ParticleBegin();	
	++It;
	(*It)->SetMass(1.5);
	ItEnd = It;
	++ItEnd;
	//shift the middle particle up 1.
	TranslateParticle Trans;		
	Trans.m_Trans[0] = -1;
	Trans.m_Trans[1] = 0;
	Trans.m_Trans[2] = 0;
	transform(It, ItEnd, It, Trans);
	UpdateParticles();
	DrawScene();
	if (m_WriteToFile)
	{		
		m_File << *m_pMS << ",";
		WriteAnalytical();
		m_File << "\n";
	}
}

void ChildView::WriteAnalytical()
{
	//1 particle connected to two springs that are fixed along the x axis. See diagram below
	//|---p---|
	//this uses the analytical soln to the equation
	//k_0 = spring constant of spring 0 = 1
	//k_1 = spring constant of spring 1 = 1
	//m = mass of particle
	//x(0) = -1
	//x'(0) = 0
	//x(t) = displacement from resting point = x(0)*cos(sqrt(K)*t), where K = (k_0+k_1)/m
	//Period = T = (2*pi)/sqrt(K)
	double K = (1.0+1.0)/1.5;
	double Disp = -1.0*cos(sqrt(K)*m_CurrentTime);
	double T = (2.0*3.141592)/sqrt(K);
	m_File << /*m_CurrentTime << "," <<*/ 3+Disp /*<< "," << Disp << "," << T << "\n"*/;
}

void ChildView::OnCreate2DGrid()
{
	// TODO: Add your command handler code here
	m_CurrentTime = 0;
	if (m_pMS) delete m_pMS;
	m_pMS = new Grid2D(3,3,3);
	m_pMS->SetDamping(0.01);
	int NumParticles = m_pMS->GetNumParticles();	
	m_Particles.resize(NumParticles);	
	MeshPtr pSphere(new Mesh);
	pSphere->LoadObj("sphere.obj");
	for (int i = 0; i < NumParticles; ++i)
	{	
		MeshPtr pMesh(new Mesh(*pSphere));
		m_Particles[i] = pMesh;
	}	

	MassSpringSystem::ParticleIterator It, ItEnd;
	It = m_pMS->ParticleBegin();
	++It;
	ItEnd = It;
	++ItEnd;	
	TranslateParticle Trans;		
	Trans.m_Trans[0] = -3;
	Trans.m_Trans[1] = 0;
	Trans.m_Trans[2] = 0;
	transform(It, ItEnd, It, Trans);
	UpdateParticles();
	DrawScene();
	if (m_WriteToFile)
	{		
		m_File << *m_pMS < "\n";
	}
}

void ChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
	{	
		m_Camera.TurnLeft();
	}	
	if (nChar == VK_RIGHT)
	{
		m_Camera.TurnRight();
	}
	if (nChar == VK_UP)
	{
		m_Camera.TurnUp();
	}
	if (nChar == VK_DOWN)
	{
		m_Camera.TurnDown();
	}
	DrawScene();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
