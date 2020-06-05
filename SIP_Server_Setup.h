#if !defined(AFX_SIP_SERVER_SETUP_H__F532704B_1672_481F_AA6A_92C024B50484__INCLUDED_)
#define AFX_SIP_SERVER_SETUP_H__F532704B_1672_481F_AA6A_92C024B50484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIP_Server_Setup.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSIP_Server_Setup dialog
#define		MAX_REG_LEVEL			4
#define		MAX_PROXY_LEVEL			3
#define		ADVANCED_EDIT_TIMER		WM_USER+2000

class CSIP_Server_Setup : public CDialog
{
// Construction
public:
	CSIP_Server_Setup(CWnd* pParent = NULL);   // standard constructor

	int		Reg_Auth_Level,Proxy_Auth_Level;
	char	Reg_Auth_Msg[200],Proxy_Auth_Msg[200];
	char	Current_Directory[_MAX_PATH];

	int		Update_SIP_Server_Param(char *Method);
	int		Advanced_Edit_Proc(bool Enable);
	int		BrowseForFolder(HWND hWnd,char *szPath);
	int		Create_Directory(char *szPath);
	int		Get_Mapping_TABLE(FILE *Fin,char *Data);
	int		Get_ReplaceCode_Table(FILE *Fin,char *Data);

// Dialog Data
	//{{AFX_DATA(CSIP_Server_Setup)
	enum { IDD = IDD_SERVER_SETUP };
	CString	m_Proxy_Auth_Msg;
	CString	m_Reg_Auth_Msg;
	CString	m_Log_Path;
	BOOL	m_Invite_Log_Check;
	BOOL	m_Call_Log_Check;
	BOOL	m_Call_Flows_Check;
	BOOL	m_Reg_Flows_Check;
	BOOL	m_Forwarding_GW_Check;
	BOOL	m_Forwarding_Mode_Check;
	BOOL	m_Progressive_Expires_Check;
	CString	m_Locking_Expires;
	BOOL	m_SubServer_Mapping_Check;
	CString	m_SubServer_Mapping_Msg;
	BOOL	m_AutoCompose_PW_Check;
	CString	m_Expire_Limit_Weight;
	BOOL	m_ReplaceCode_Check;
	CString	m_ReplaceCode_Msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSIP_Server_Setup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSIP_Server_Setup)
	afx_msg void OnRegister_Level();
	afx_msg void OnProxy_Auth_Level();
	virtual BOOL OnInitDialog();
	afx_msg void OnLog_Path();
	afx_msg void OnInvite_Log_Check();
	afx_msg void OnCall_Log_Check();
	afx_msg void OnCall_Flows_Check();
	afx_msg void OnReg_Flows_Check();
	afx_msg void OnAdvanced_Edit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBrowseForld();
	afx_msg void OnClose();
	afx_msg void OnForwarding_GW();
	afx_msg void OnForwarding_Mode();
	afx_msg void OnProgressive_Expires();
	afx_msg void OnLocking_Expires();
	afx_msg void OnEdit_Mapping_List();
	afx_msg void OnSubServer_Table_Reload();
	afx_msg void OnAuto_Compose_PW();
	afx_msg void OnExpire_Limit_Weight();
	afx_msg void OnReplaceCode_Check();
	afx_msg void OnReplace_Phone_Table();
	afx_msg void OnReplace_Phone_Reload();
	afx_msg void OnSubServer_Mapping_Check();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIP_SERVER_SETUP_H__F532704B_1672_481F_AA6A_92C024B50484__INCLUDED_)
