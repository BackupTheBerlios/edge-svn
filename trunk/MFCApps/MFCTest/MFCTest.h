// MFCTest.h : main header file for the MFCTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../../Render/Mesh.hpp"

// MFCTestApp:
// See MFCTest.cpp for the implementation of this class
//

class MFCTestApp : public CWinApp
{

public:
	MFCTestApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern MFCTestApp theApp;