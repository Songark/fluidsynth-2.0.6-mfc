
// fluidsynth-2.0.6-mfc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cfluidsynth206mfcApp:
// See fluidsynth-2.0.6-mfc.cpp for the implementation of this class
//

class Cfluidsynth206mfcApp : public CWinApp
{
public:
	Cfluidsynth206mfcApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cfluidsynth206mfcApp theApp;
