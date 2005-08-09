#include "stdafx.h"
#include "./BoundingBoxes.h"
#include "../../Render/Render.hpp"
#include "../../Physics/Physics.hpp"


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

HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInst;									// This holds the global hInstance for UnregisterClass() in DeInit()
Camera g_Cam;
Node::NodePtr g_pRoot(new Node);
double g_TotalTime = 0;
const double g_FixedTimeStep = 0.01;

int APIENTRY _tWinMain(HINSTANCE hInstance,
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
	LoadString(hInstance, IDC_BOUNDINGBOXES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BOUNDINGBOXES);
		

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

			DWORD Time = GetTickCount();
			DWORD dt = Time - LastTime;			
			if (dt > 100) 
				dt = 100;			
			if (LastTime != -1)
			{										
				//g_TotalTime += dt/1000.0;
				g_TotalTime += g_FixedTimeStep;
				//UpdateScene(dt/1000.0);
				//update the scene by a fixed time step
				UpdateScene(g_FixedTimeStep);
				RenderScene();				
			}
			LastTime = Time;
			//if (g_TotalTime > 40.0)
			//{
			//	PostQuitMessage(0);
			//}
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDC_BOUNDINGBOXES);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_BOUNDINGBOXES;
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
	glClearColor((GLclampf)0.3, (GLclampf)0.3, (GLclampf)0.3, 0);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer	
	ublas::vector<double> LookAt(3);
	LookAt = g_Cam.GetPostion() + 10.0*g_Cam.GetDir();
	gluLookAt(g_Cam.GetPostion()[0], g_Cam.GetPostion()[1], g_Cam.GetPostion()[2], 
		LookAt[0], LookAt[1], LookAt[2], 
		g_Cam.GetUp()[0], g_Cam.GetUp()[1], g_Cam.GetUp()[2]);		
	RenderVisitor Vis;
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

	switch (message) 
	{
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

	case WM_CHAR:
		switch (wParam)
		{
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

	case WM_KEYUP:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_SIZE:										// If the window is resized
		SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect);				// Get the window rectangle		
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
		
	UnregisterClass("INSECTTHING", g_hInst);		// Free the window class
	
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
	PreRenderVisitor PR;
	PR.InitGL();

	Material::MaterialPtr pMatGreen(new Material);
	Material::MaterialPtr pMatRed(new Material);
	Material::MaterialPtr pMatBlue(new Material);
	Material::MaterialPtr pMatPurple(new Material);
	AmbientLight::AmbientLightPtr pALight(new AmbientLight);
	Mesh::MeshPtr pMesh(new Mesh);
	Mesh::MeshPtr pMesh1(new Mesh);
	Translate::TranslatePtr pTranslate(new Translate);
	Translate::TranslatePtr pTranslate1(new Translate);
	Scale::ScalePtr pScale(new Scale);
	Scale::ScalePtr pScale1(new Scale);
	Rotate::RotatePtr pRotate(new Rotate);
	BoundNode::BoundNodePtr pBNode(new BoundNode);
	BoundNode::BoundNodePtr pBNode1(new BoundNode);
	BoundNode::BoundNodePtr pBNode2(new BoundNode);
	BoundsCalculator::BoundsCalculatorPtr pBCalc(new BoundsCalculator);

	pRotate->SetAxis(0,0,1);
	pRotate->SetAngle(Pi/2.0);
	pTranslate->SetTranslate(2,-2,0);
	pTranslate1->SetTranslate(-2,-2,0);
	pScale->SetScale(0.5, 0.5, 0.5);
	pScale1->SetScale(0.2, 0.2, 0.2);
	//setup light properties
	pALight->SetAmbient(RGBColor(0.5, 0.5, 0.5));
	pALight->SetDiffuse(RGBColor(0.2, 0.2, 0.2));
	//pDLight->SetDiffuse(RGBColor(0.8, 0.8, 0.8));
	//pDLight->SetAmbient(RGBColor(0.1, 0.1, 0.1));

	pMatGreen->SetDiffuse(RGBColor(0, 0.2, 0));
	pMatGreen->SetAmbient(RGBColor(0.2, 0.5, 0.2));
	pMatRed->SetDiffuse(RGBColor(0.5, 0.0, 0));
	pMatRed->SetAmbient(RGBColor(0.5, 0.2, 0.2));
	pMatBlue->SetDiffuse(RGBColor(0.0, 0.0, 0.5));
	pMatBlue->SetAmbient(RGBColor(0.2, 0.2, 0.5));
	pMatPurple->SetDiffuse(RGBColor(0.5, 0.0, 0.5));
	pMatPurple->SetAmbient(RGBColor(0.5, 0.2, 0.5));
	
	pMesh->LoadObj("./plane.obj");
	pMesh1->LoadObj("./plane.obj");

	pMatBlue->AddChild(pMesh);
	pScale->AddChild(pMatBlue);
	pScale1->AddChild(pMesh1);
	pTranslate->AddChild(pScale);
	pTranslate1->AddChild(pScale1);
	pRotate->AddChild(pMatBlue);
	pMatGreen->AddChild(pTranslate1);	
	pBNode->AddChild(pRotate);	
	//pMatRed->AddChild(pBNode);
	pMatRed->AddChild(pRotate);
	pBNode1->AddChild(pMatRed);	
	pBNode1->AddChild(pMatGreen);
	pMatPurple->AddChild(pBNode1);
	pALight->AddChild(pMatPurple);
	g_pRoot->AddChild(pALight);
}

void UpdateScene(float dt)
{
	TimeStepVisitor<Edge::RK4Solver> TS;
	BoundsCalculator BC;
	TS.SetTime(dt);
	g_pRoot->ApplyVisitor(&TS);		
	g_pRoot->ApplyVisitor(&BC);
}
