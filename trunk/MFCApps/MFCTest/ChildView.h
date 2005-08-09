// ChildView.h : interface of the ChildView class
//
#pragma once
#include <fstream>
#include <sstream>
#include "../../Render/Mesh.hpp"
#include "../../Render/OpenGLRenderer.hpp"
#include "../../Render/Camera.hpp"
#include "../../Physics/Grid1D.hpp"
#include "../../Physics/Grid2D.hpp"

using namespace Edge;

class ChildView : public CView
{
// Attributes
private:
	Camera		m_Camera;
	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;
	vector<Mesh::MeshPtr> m_Particles;	
	MassSpringSystem* m_pMS;
	OpenGLRenderer m_OGL;
	HGLRC   m_OpenGLRenderingContext;
	//float EyeX, EyeY, EyeZ;
	float	m_FarPlane;
	float	m_NearPlane;	
	double m_TimeStep;
	double m_CurrentTime;
	bool m_WriteToFile;
	std::fstream m_File;
	
// Operations
public:
	ChildView();
	//DECLARE_DYNCREATE(ChildView)

	void Init();
	void CreateRGBPalette(void);
	BOOL bSetupPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	void DrawScene(void);
	void CreateGrid(int Dim);
	void UpdateParticles();
	void WriteAnalytical();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChildView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ChildView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL    m_play;

// Generated message map functions
protected:
	//{{AFX_MSG(ChildView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);//	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActionsStep();
	afx_msg void OnActionsPlay();
	afx_msg void OnActionsStop();
	afx_msg void OnActionsWriteToFile();
	afx_msg void OnCreate1DGrid();
	afx_msg void OnCreate2DGrid();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


