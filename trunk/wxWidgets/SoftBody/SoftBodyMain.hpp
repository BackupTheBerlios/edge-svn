

#ifndef _WX_CUBE_H_
#define _WX_CUBE_H_

#include "wx/glcanvas.h"
#include "SoftBodyNode.hpp"

// Define a new application type
class SoftBodyApp: public wxApp
{
public:
	bool OnInit(void);
};

// Define a new frame type
class GLCanvas;
class SoftBodyFrame: public wxFrame
{
public:
	SoftBodyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
		const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

	void OnExit(wxCommandEvent& event);
	void OnNewWindow(wxCommandEvent& event);

public:
	GLCanvas*    m_canvas;

	DECLARE_EVENT_TABLE()
};

class GLCanvas: public wxGLCanvas
{
	friend class SoftBodyFrame;
public:
	GLCanvas(wxWindow *parent, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = "GLCanvas");
	GLCanvas(wxWindow *parent, const GLCanvas &other,
		const wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = "GLCanvas" );

	~GLCanvas(void);

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnEnterWindow( wxMouseEvent& event );


	void BuildScene(void);
	void UpdateScene(float dt);
	void Render( void );
	void InitGL(void);
	void Rotate( GLfloat deg );
	static GLfloat CalcRotateSpeed( unsigned long acceltime );
	static GLfloat CalcRotateAngle( unsigned long lasttime,
		unsigned long acceltime );
	void Action( long code, unsigned long lasttime,
		unsigned long acceltime );

private:
	bool   m_init;
	GLuint m_gllist;
	long   m_rleft;
	long   m_rright;

	static unsigned long  m_secbase;
	static int            m_TimeInitialized;
	static unsigned long  m_xsynct;
	static unsigned long  m_gsynct;

	long           m_Key;
	unsigned long  m_StartTime;
	unsigned long  m_LastTime;
	unsigned long  m_LastRedraw;

	Edge::Node::NodePtr m_pRoot;
	SoftBodyNode::SoftBodyNodePtr m_pSBNode;
	double m_TotalTime;
	const double m_FixedTimeStep;
	Edge::Camera m_Cam;

	DECLARE_EVENT_TABLE()
};


#endif

