// PENDULUM.cpp : Defines the entry point for the application.
//

#include "../../Render/Render.hpp"
#include "../../Physics/Particle.hpp"
#include "./Pendulum.h"
#include "./PendulumNode.hpp"
#include "./PendulumNodeWriter.hpp"
#include "./PendulumRenderer.hpp"


using namespace Edge;
using namespace boost;
using namespace boost::numeric;

#define MAX_LOADSTRING 100
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//PendulumNodeWriter g_PNWriter;
bool g_Pause=false;
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInst;									// This holds the global hInstance for UnregisterClass() in DeInit()
Camera g_Cam;
NodePtr g_pRoot(new Node);
PendulumNode::PendulumNodePtr g_pPenNode(new PendulumNode);
double g_TotalTime = 0;
const double g_FixedTimeStep = 0.01;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	static DWORD LastTime = -1;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PENDULUM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PENDULUM);
		
	int err = SetTimer(g_hWnd, 1, 10, NULL);
	if (err == 0)
	{
		OutputDebugString("shit\n");
	}
	while(1)
	{		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			RenderScene();
        } 
	}

	DeInit();// Clean up and free all allocated memory
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDC_PENDULUM);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_PENDULUM;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

void DrawTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-1,0,0);
	glVertex3f(1,0,0);
	glVertex3f(0,1,0);
	glEnd();
}

void RenderScene() 
{
	int i=0;		
	glClearColor(0,0,0,0);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer	
	ublas::vector<double> LookAt(3);
	LookAt = g_Cam.GetPostion() + 10.0*g_Cam.GetDir();
	gluLookAt(g_Cam.GetPostion()[0], g_Cam.GetPostion()[1], g_Cam.GetPostion()[2], 
		LookAt[0], LookAt[1], LookAt[2], 
		g_Cam.GetUp()[0], g_Cam.GetUp()[1], g_Cam.GetUp()[2]);		
	PendulumRenderer Vis;
	g_pRoot->ApplyVisitor(&Vis);
	SwapBuffers(g_hDC);									// Swap the backbuffers to the foreground
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	g_hInst = hInstance; // Store instance handle in our global variable

	g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	// INIT OpenGL
	Init();	

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int err;
	static UINT_PTR TimerID=1;

	switch (message) 
	{
	case WM_CREATE:
		
		return 0;

	case WM_TIMER:		
		TimerUpdate();
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;		

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		//RenderScene();
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_SIZE:										// If the window is resized
		SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect);				// Get the window rectangle		
        break;  
	
	case WM_CHAR:
		switch (wParam)
		{
		case 'p':
			g_Pause = !g_Pause;
			return 0;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_DOWN:
				g_Cam.MoveBackward();
				return 0;
			case VK_UP:
				g_Cam.MoveForward();
				return 0;
			case VK_RIGHT:
				g_Cam.TurnRight();
				return 0;
			case VK_LEFT:
				g_Cam.TurnLeft();
				return 0;	
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

bool SetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = SCREEN_DEPTH;						// Here we use our #define for the color bits
    pfd.cDepthBits = SCREEN_DEPTH;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
	// This sets the pixel format that we extracted from above
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE;										// Return a success!
}

void SizeOpenGLScreen(int width, int height)			// Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero error
	{
		height=1;										// Make the Height Equal One
	}

	glViewport(0,0,width,height);						// Make our viewport the whole window
														// We could make the view smaller inside
														// Our window if we wanted too.
														// The glViewport takes (x, y, width, height)
														// This basically means, what our our drawing boundries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  The closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

///////////////////////////////// INITIALIZE GL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles all the initialization for OpenGL.
/////
///////////////////////////////// INITIALIZE GL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height) 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if (!SetupPixelFormat(g_hDC))						// This sets our pixel format/information
        PostQuitMessage (0);							// If there's an error, quit

    g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
    wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	SizeOpenGLScreen(width, height);					// Setup the screen translations and viewport
}

void DeInit()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory
		
	UnregisterClass("PENDULUM", g_hInst);		// Free the window class
	
	PostQuitMessage (0);								// Post a QUIT message to the window
}

void Init()
{	
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect	
	ublas::vector<double> Pos(3);
	ublas::vector<double> Left(3);
	ublas::vector<double> Up(3);
	ublas::vector<double> Dir(3);
	Pos[0] = 0; Pos[1] = 0; Pos[2] = 10;
	g_Cam.SetPosition(Pos);
	Left[0] = -1; Left[1] = 0; Left[2] = 0;
	Up[0] = 0; Up[1] = 1; Up[2] = 0;
	Dir[0] = 0; Dir[1] = 0; Dir[2] = -1;
	g_Cam.SetAxes(Left, Up, Dir);	
	BuildScene();
}

void BuildScene()
{
	double L0, L1, L2, L3, L4, L5, L6;
	double Theta0, Theta1, Theta2, Theta3, Theta4, Theta5, Theta6;
	L0 = 1;
	L1 = 1;
	L2 = 1;
	L3 = 1;
	L4 = 1;
	L5 = 1;
	L6 = 1;
	Theta0 = Pi/2.0;
	Theta1 = Pi/2.0;
	Theta2 = Pi/2.0;
	Theta3 = Pi/2.0;
	Theta4 = Pi/2.0;
	Theta5 = Pi/2.0;
	Theta6 = Pi/2.0;

	PreRenderVisitor PR;
	PR.InitGL();
	Chain::ChainPtr pChain(new Chain);
	bnu::vector<double> Force(3);
	Force[0] = 0;
	Force[1] = -9.8;
	//Force[1] = 0;
	Force[2] = 0;	
	Edge::ParticlePtr pP0(new Particle);
	Edge::ParticlePtr pP1(new Particle);	
	Edge::ParticlePtr pP2(new Particle);	
	Edge::ParticlePtr pP3(new Particle);
	Edge::ParticlePtr pP4(new Particle);
	Edge::ParticlePtr pP5(new Particle);	
	Edge::ParticlePtr pP6(new Particle);	
	Edge::ParticlePtr pP7(new Particle);
	DirectionLightPtr pDLight1(new DirectionLight(0));
	DirectionLightPtr pDLight(new DirectionLight(1));
	
	//setup light properties	
	//pALight->SetDirection(0,0,-1;)g_
	pDLight1->SetDiffuse(RGBColor(0.8, 0.8, 0.8));
	pDLight1->SetAmbient(RGBColor(0.3, 0.3, 0.3));
	pDLight1->SetDirection(0,-1,0);
	pDLight->SetDiffuse(RGBColor(0.3, 0.3, 0.3));
	pDLight->SetAmbient(RGBColor(0.3, 0.3, 0.3));
	pDLight->SetDirection(0,0,-1);

	
	//set positions of particles
	pP0->SetPosition(0,2.5,0);
	pP1->SetPosition(pP0->GetPosition()[0] + sin(Theta0), pP0->GetPosition()[1] - cos(Theta0),0);
	pP2->SetPosition(pP1->GetPosition()[0] + sin(Theta1), pP1->GetPosition()[1] - cos(Theta1), 0);
	pP3->SetPosition(pP2->GetPosition()[0] + sin(Theta2), pP2->GetPosition()[1] - cos(Theta2), 0);
	pP4->SetPosition(pP3->GetPosition()[0] + sin(Theta3), pP3->GetPosition()[1] - cos(Theta3), 0);
	pP5->SetPosition(pP4->GetPosition()[0] + sin(Theta4), pP4->GetPosition()[1] - cos(Theta4), 0);
	pP6->SetPosition(pP5->GetPosition()[0] + sin(Theta5), pP5->GetPosition()[1] - cos(Theta5), 0);
	

	//build chain system	
	pChain->ResizeParticleContainer(7);
	pChain->ResizeForceContainer(7);
	pChain->SetParticle(0, pP0);
	pChain->SetParticle(1, pP1);
	pChain->SetParticle(2, pP2);
	pChain->SetParticle(3, pP3);
	pChain->SetParticle(4, pP4);
	pChain->SetParticle(5, pP5);
	pChain->SetParticle(6, pP6);
	pChain->SetForceAllParticles(Force);
	pChain->BuildSystem();
	pChain->SetDampingConstant(0.5);
	g_pPenNode->Setup(pChain);

	//build scene graph		
	pDLight->AddChild(g_pPenNode);
	pDLight1->AddChild(pDLight);
	g_pRoot->AddChild(pDLight1);

//	g_PNWriter.Init(g_pPenNode);
}

void UpdateScene(float dt)
{
	TimeStepVisitor<Edge::RK4Solver> TS;
	TS.SetTime(dt);
	//g_PNWriter.SetTime(g_TotalTime);
	g_pRoot->ApplyVisitor(&TS);		
}

void TimerUpdate()
{
	if (!g_Pause)
	{
		g_TotalTime += g_FixedTimeStep;
		UpdateScene(g_FixedTimeStep);	
	}
}