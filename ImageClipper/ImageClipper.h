
// ImageClipper.h : main header file for the ImageClipper application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CImageClipperApp:
// See ImageClipper.cpp for the implementation of this class
//

class CImageClipperApp : public CWinApp
{
public:
	CImageClipperApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImageClipperApp theApp;
