// CallServerDlg.h : header file
//

#if !defined(AFX_CALLSERVERDLG_H__12EE796E_5D9F_4D39_B0B7_6339F2C62A9E__INCLUDED_)
#define AFX_CALLSERVERDLG_H__12EE796E_5D9F_4D39_B0B7_6339F2C62A9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdHeader.h"
#include "SIP_Server.h"

#define  START_EVENT  WM_USER+200

/////////////////////////////////////////////////////////////////////////////
// CCallServerDlg dialog

class CCallServerDlg : public CDialog
{
// Construction
public:
	CCallServerDlg(CWnd* pParent = NULL);	// standard constructor

	CSIP_Server		*lpSIP_Server;
    char			ErrMsg[100];
	CString			m_Edit1;


// Dialog Data
	//{{AFX_DATA(CCallServerDlg)
	enum { IDD = IDD_CALLSERVER_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallServerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCallServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLSERVERDLG_H__12EE796E_5D9F_4D39_B0B7_6339F2C62A9E__INCLUDED_)
