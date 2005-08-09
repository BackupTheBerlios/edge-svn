/////////////////////////////////////////////////////////////////////////////
// Name:        cube.cpp
// Purpose:     wxGLCanvas demo program
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: cube.cpp,v 1.9 2002/08/31 22:30:50 GD Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/log.h"

#if !wxUSE_GLCANVAS
#error Please set wxUSE_GLCANVAS to 1 in setup.h.
#endif


#include "../../Render/Render.hpp"
#include "../../Physics/Physics.hpp"
#include "SoftBodyMain.hpp"
#include "SoftBodyNode.hpp"

#ifndef __WXMSW__     // for wxStopWatch, see remark below
#if defined(__WXMAC__) && !defined(__DARWIN__)
#include <utime.h>
#include <unistd.h>
#else
#include <sys/time.h>
#include <sys/unistd.h>
#endif
#else
#include <sys/timeb.h>
#endif


#define ID_NEW_WINDOW 10000
#define ID_DEF_ROTATE_LEFT_KEY 10001
#define ID_DEF_ROTATE_RIGHT_KEY 10002

using namespace Edge;



unsigned long wxStopWatch( unsigned long *sec_base )
{
	unsigned long secs,msec;

#if defined(__WXMSW__)
	struct timeb tb;
	ftime( &tb );
	secs = tb.time;
	msec = tb.millitm;
#elif defined(__WXMAC__) && !defined(__DARWIN__)
	wxLongLong tl = wxGetLocalTimeMillis();
	secs = (unsigned long) (tl.GetValue() / 1000);
	msec = (unsigned long) (tl.GetValue() - secs*1000);
#else
	// think every unice has gettimeofday
	struct timeval tv;
	gettimeofday( &tv, (struct timezone *)NULL );
	secs = tv.tv_sec;
	msec = tv.tv_usec/1000;
#endif

	if( *sec_base == 0 )
		*sec_base = secs;

	return( (secs-*sec_base)*1000 + msec );
}

/*----------------------------------------------------------------
Implementation of Test-GLCanvas
-----------------------------------------------------------------*/

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
EVT_SIZE(GLCanvas::OnSize)
EVT_PAINT(GLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
EVT_KEY_DOWN( GLCanvas::OnKeyDown )
EVT_KEY_UP( GLCanvas::OnKeyUp )
EVT_ENTER_WINDOW( GLCanvas::OnEnterWindow )
END_EVENT_TABLE()

unsigned long  GLCanvas::m_secbase = 0;
int            GLCanvas::m_TimeInitialized = 0;
unsigned long  GLCanvas::m_xsynct;
unsigned long  GLCanvas::m_gsynct;

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id,
						   const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style, name ),
	m_pRoot(new Node),
	m_pSBNode(new SoftBodyNode),
	m_TotalTime(0),
	m_FixedTimeStep(0.01)
{
	m_init = FALSE;
	m_gllist = 0;
	m_rleft = WXK_LEFT;
	m_rright = WXK_RIGHT;
}

GLCanvas::GLCanvas(wxWindow *parent, const GLCanvas &other,
						   wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
						   const wxString& name ) :
	wxGLCanvas(parent, other.GetContext(), id, pos, size, style, name  ),
	m_pRoot(new Node),
	m_pSBNode(new SoftBodyNode),
	m_TotalTime(0),
	m_FixedTimeStep(0.01)
{
	m_init = FALSE;
	m_gllist = other.m_gllist;    /* share display list */
	m_rleft = WXK_LEFT;
	m_rright = WXK_RIGHT;
}

GLCanvas::~GLCanvas()
{
}

void GLCanvas::Render()
{
	wxPaintDC dc(this);

#ifndef __WXMOTIF__
	if (!GetContext()) return;
#endif

	SetCurrent();
	/* init OpenGL once, but after SetCurrent */
	if (!m_init)
	{
		InitGL();
		m_init = TRUE;
	}

	glClearColor((GLclampf)0.3, (GLclampf)0.3, (GLclampf)0.3, 0);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer	
	ublas::vector<double> LookAt(3);
	LookAt = m_Cam.GetPostion() + 10.0*m_Cam.GetDir();
	gluLookAt(m_Cam.GetPostion()[0], m_Cam.GetPostion()[1], m_Cam.GetPostion()[2], 
		LookAt[0], LookAt[1], LookAt[2], 
		m_Cam.GetUp()[0], m_Cam.GetUp()[1], m_Cam.GetUp()[2]);		
	RenderVisitor Vis;
	m_pRoot->ApplyVisitor(&Vis);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);
	glMatrixMode(GL_MODELVIEW);

	/* clear color and depth buffers */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if( m_gllist == 0 )
	//{
	//	m_gllist = glGenLists( 1 );
	//	glNewList( m_gllist, GL_COMPILE_AND_EXECUTE );
	//	/* draw six faces of a cube */
	//	glBegin(GL_QUADS);
	//	glNormal3f( 0.0F, 0.0F, 1.0F);
	//	glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
	//	glVertex3f(-0.5F,-0.5F, 0.5F); glVertex3f( 0.5F,-0.5F, 0.5F);

	//	glNormal3f( 0.0F, 0.0F,-1.0F);
	//	glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f(-0.5F, 0.5F,-0.5F);
	//	glVertex3f( 0.5F, 0.5F,-0.5F); glVertex3f( 0.5F,-0.5F,-0.5F);

	//	glNormal3f( 0.0F, 1.0F, 0.0F);
	//	glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f( 0.5F, 0.5F,-0.5F);
	//	glVertex3f(-0.5F, 0.5F,-0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);

	//	glNormal3f( 0.0F,-1.0F, 0.0F);
	//	glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f( 0.5F,-0.5F,-0.5F);
	//	glVertex3f( 0.5F,-0.5F, 0.5F); glVertex3f(-0.5F,-0.5F, 0.5F);

	//	glNormal3f( 1.0F, 0.0F, 0.0F);
	//	glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f( 0.5F,-0.5F, 0.5F);
	//	glVertex3f( 0.5F,-0.5F,-0.5F); glVertex3f( 0.5F, 0.5F,-0.5F);

	//	glNormal3f(-1.0F, 0.0F, 0.0F);
	//	glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f(-0.5F,-0.5F, 0.5F);
	//	glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F,-0.5F);
	//	glEnd();

	//	glEndList();
	//}
	//else
	//	glCallList( m_gllist );

	glFlush();
	SwapBuffers();
}

void GLCanvas::OnEnterWindow( wxMouseEvent& event )
{
	SetFocus();
}

void GLCanvas::OnPaint( wxPaintEvent& event )
{
	Render();
}

void GLCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int w, h;
	GetClientSize(&w, &h);
#ifndef __WXMOTIF__
	if (GetContext())
#endif
	{
		SetCurrent();
		glViewport(0, 0, (GLint) w, (GLint) h);
	}
}

void GLCanvas::OnEraseBackground(wxEraseEvent& event)
{
	// Do nothing, to avoid flashing.
}

void GLCanvas::InitGL()
{

	SetCurrent();

	/* set viewing projection */
	glMatrixMode(GL_PROJECTION);
	glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

	/* position viewer */
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0F, 0.0F, -2.0F);

	/* position object */
	glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
	glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	ublas::vector<double> Pos(3);
	ublas::vector<double> Left(3);
	ublas::vector<double> Up(3);
	ublas::vector<double> Dir(3);
	Pos[0] = 0; Pos[1] = 0; Pos[2] = 10;
	m_Cam.SetPosition(Pos);
	Left[0] = -1; Left[1] = 0; Left[2] = 0;
	Up[0] = 0; Up[1] = 1; Up[2] = 0;
	Dir[0] = 0; Dir[1] = 0; Dir[2] = -1;
	m_Cam.SetAxes(Left, Up, Dir);	
	BuildScene();
}

GLfloat GLCanvas::CalcRotateSpeed( unsigned long acceltime )
{
	GLfloat t,v;

	t = ((GLfloat)acceltime) / 1000.0f;

	if( t < 0.5f )
		v = t;
	else if( t < 1.0f )
		v = t * (2.0f - t);
	else
		v = 0.75f;

	return(v);
}

GLfloat GLCanvas::CalcRotateAngle( unsigned long lasttime,
									  unsigned long acceltime )
{
	GLfloat t,s1,s2;

	t = ((GLfloat)(acceltime - lasttime)) / 1000.0f;
	s1 = CalcRotateSpeed( lasttime );
	s2 = CalcRotateSpeed( acceltime );

	return( t * (s1 + s2) * 135.0f );
}

void GLCanvas::Action( long code, unsigned long lasttime,
						  unsigned long acceltime )
{
	GLfloat angle = CalcRotateAngle( lasttime, acceltime );

	if (code == m_rleft)
		Rotate( angle );
	else if (code == m_rright)
		Rotate( -angle );
}

void GLCanvas::OnKeyDown( wxKeyEvent& event )
{
	long evkey = event.KeyCode();
	if (evkey == 0) return;

	if (!m_TimeInitialized)
	{
		m_TimeInitialized = 1;
		m_xsynct = event.m_timeStamp;
		m_gsynct = wxStopWatch(&m_secbase);

		m_Key = evkey;
		m_StartTime = 0;
		m_LastTime = 0;
		m_LastRedraw = 0;
	}

	unsigned long currTime = event.m_timeStamp - m_xsynct;

	if (evkey != m_Key)
	{
		m_Key = evkey;
		m_LastRedraw = m_StartTime = m_LastTime = currTime;
	}

	if (currTime >= m_LastRedraw)      // Redraw:
	{
		Action( m_Key, m_LastTime-m_StartTime, currTime-m_StartTime );

#if defined(__WXMAC__) && !defined(__DARWIN__)
		m_LastRedraw = currTime;    // wxStopWatch() doesn't work on Mac...
#else
		m_LastRedraw = wxStopWatch(&m_secbase) - m_gsynct;
#endif
		m_LastTime = currTime;
	}

	event.Skip();
}

void GLCanvas::OnKeyUp( wxKeyEvent& event )
{
	m_Key = 0;
	m_StartTime = 0;
	m_LastTime = 0;
	m_LastRedraw = 0;

	event.Skip();
}

void GLCanvas::Rotate( GLfloat deg )
{
	SetCurrent();

	glMatrixMode(GL_MODELVIEW);
	glRotatef((GLfloat)deg, 0.0F, 0.0F, 1.0F);
	Refresh(FALSE);
}

void GLCanvas::BuildScene()
{
	PreRenderVisitor PR;
	PR.InitGL();

	Material::MaterialPtr pMat(new Material);
	Material::MaterialPtr pMatGreen(new Material);
	DirectionLight::DirectionLightPtr pDLight(new DirectionLight);
	AmbientLight::AmbientLightPtr pALight(new AmbientLight);
	Translate::TranslatePtr pTranslate(new Translate);
	Scale::ScalePtr pScaleGround(new Scale);

	//setup light properties
	pALight->SetAmbient(RGBColor(0.5, 0.5, 0.5));
	pALight->SetDiffuse(RGBColor(0.2, 0.2, 0.2));
	pDLight->SetDirection(0, 0, -1);
	pDLight->SetDiffuse(RGBColor(0.0, 0.0, 0.0));
	pDLight->SetAmbient(RGBColor(0.0, 0.0, 0.0));

	//setup material properties
	pMat->SetDiffuse(RGBColor(0.8, 0.2, 0.2));
	pMat->SetAmbient(RGBColor(0.5, 0.0, 0.0));
	
	pMatGreen->SetDiffuse(RGBColor(0, 0.5, 0));
	pMatGreen->SetAmbient(RGBColor(0, 0.5, 0));

	m_pSBNode->Setup();

	pMat->AddChild(m_pSBNode);
	pALight->AddChild(pMat);	
	pDLight->AddChild(pALight);
	//pALight->AddChild(pMat);
	m_pRoot->AddChild(pDLight);
}

void GLCanvas::UpdateScene(float dt)
{
	TimeStepVisitor<Edge::RK4Solver> TS;
	TS.SetTime(dt);
	m_pRoot->ApplyVisitor(&TS);		
}


/* -----------------------------------------------------------------------
Main Window
-------------------------------------------------------------------------*/

BEGIN_EVENT_TABLE(SoftBodyFrame, wxFrame)
EVT_MENU(wxID_EXIT, SoftBodyFrame::OnExit)
EVT_MENU( ID_NEW_WINDOW, SoftBodyFrame::OnNewWindow)
END_EVENT_TABLE()

// My frame constructor
SoftBodyFrame::SoftBodyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
				 const wxSize& size, long style)
				 : wxFrame(frame, -1, title, pos, size, style)
{
	m_canvas = NULL;
}

// Intercept menu commands
void SoftBodyFrame::OnExit(wxCommandEvent& event)
{
	Destroy();
}

void SoftBodyFrame::OnNewWindow(wxCommandEvent& event)
{
	SoftBodyFrame *frame = new SoftBodyFrame(NULL, "Cube OpenGL Demo Clone",
		wxPoint(50, 50), wxSize(400, 300));
	// Give it an icon
#ifdef __WXMSW__
	frame->SetIcon(wxIcon("mondrian"));
#endif

	// Make a menubar
	wxMenu *winMenu = new wxMenu;

	winMenu->Append(wxID_EXIT, "&Close");
	winMenu->Append(ID_NEW_WINDOW, "&New" );
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(winMenu, "&Window");

	winMenu = new wxMenu;
	winMenu->Append(ID_DEF_ROTATE_LEFT_KEY, "Rotate &left");
	winMenu->Append(ID_DEF_ROTATE_RIGHT_KEY, "Rotate &right");
	menuBar->Append(winMenu, "&Key");

	frame->SetMenuBar(menuBar);

	frame->m_canvas = new GLCanvas( frame, *m_canvas, -1,
		wxDefaultPosition, wxDefaultSize );	
	// Show the frame
	frame->Show(TRUE);
}


/*------------------------------------------------------------------
Application object ( equivalent to main() )
------------------------------------------------------------------ */

IMPLEMENT_APP(SoftBodyApp)

bool SoftBodyApp::OnInit(void)
{
	wxLog::SetTraceMask(wxTraceMessages);

	// Create the main frame window
	SoftBodyFrame *frame = new SoftBodyFrame(NULL, "Cube OpenGL Demo", wxPoint(50, 50),
		wxSize(400, 300));
	// Give it an icon
#ifdef wx_msw
	frame->SetIcon(wxIcon("mondrian"));
#endif

	// Make a menubar
	wxMenu *winMenu = new wxMenu;

	winMenu->Append(wxID_EXIT, "&Close");
	winMenu->Append(ID_NEW_WINDOW, "&New" );
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(winMenu, "&Window");

	winMenu = new wxMenu;
	winMenu->Append(ID_DEF_ROTATE_LEFT_KEY, "Rotate &left");
	winMenu->Append(ID_DEF_ROTATE_RIGHT_KEY, "Rotate &right");
	menuBar->Append(winMenu, "&Key");

	frame->SetMenuBar(menuBar);

	frame->m_canvas = new GLCanvas(frame, -1, wxDefaultPosition, wxDefaultSize);
	// Show the frame
	frame->Show(TRUE);

	return TRUE;
}

