// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C5DE7BE9_7AEC_4EEE_8AB5_17D175D26DB1__INCLUDED_)
#define AFX_MAINFRM_H__C5DE7BE9_7AEC_4EEE_8AB5_17D175D26DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameView.h"


class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CGameView    m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnStart();
	afx_msg void OnReturn();
	afx_msg void OnClose();
	afx_msg void OnUpdateEasy(CCmdUI* pCmdUI);
	afx_msg void OnEasy();
	afx_msg void OnMiddle();
	afx_msg void OnUpdateMiddle(CCmdUI* pCmdUI);
	afx_msg void OnHard();
	afx_msg void OnUpdateHard(CCmdUI* pCmdUI);
	afx_msg void OnSudoguize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C5DE7BE9_7AEC_4EEE_8AB5_17D175D26DB1__INCLUDED_)
