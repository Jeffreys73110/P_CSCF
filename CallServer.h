// CallServer.h : main header file for the CALLSERVER application
//

#if !defined(AFX_CALLSERVER_H__D93A6597_BA55_4258_B00E_B7FD20F6C5D4__INCLUDED_)
#define AFX_CALLSERVER_H__D93A6597_BA55_4258_B00E_B7FD20F6C5D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCallServerApp:
// See CallServer.cpp for the implementation of this class
//

class CCallServerApp : public CWinApp
{
public:
	CCallServerApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCallServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLSERVER_H__D93A6597_BA55_4258_B00E_B7FD20F6C5D4__INCLUDED_)
