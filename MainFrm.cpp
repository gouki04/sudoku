// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Sudo.h"

#include "MainFrm.h"
#include "MapManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_START, OnStart)
	ON_COMMAND(ID_RETURN, OnReturn)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_EASY, OnUpdateEasy)
	ON_COMMAND(ID_EASY, OnEasy)
	ON_COMMAND(ID_MIDDLE, OnMiddle)
	ON_UPDATE_COMMAND_UI(ID_MIDDLE, OnUpdateMiddle)
	ON_COMMAND(ID_HARD, OnHard)
	ON_UPDATE_COMMAND_UI(ID_HARD, OnUpdateHard)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

// 	if (!m_wndStatusBar.Create(this) ||
// 		!m_wndStatusBar.SetIndicators(indicators,
// 		  sizeof(indicators)/sizeof(UINT)))
// 	{
// 		TRACE0("Failed to create status bar\n");
// 		return -1;      // fail to create
// 	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.lpszName = "Êý¶À";

	CRect rcArea;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	CRect rect(0, 0, 288, 288);
	::AdjustWindowRectEx(&rect, cs.style, TRUE, cs.dwExStyle);

	cs.cx = rect.Width();
	cs.cy = rect.Height();

	cs.x = rcArea.left + (rcArea.Width() - cs.cx) / 2;
	cs.y = rcArea.top + (rcArea.Height() - cs.cy) / 2;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnStart() 
{
	// TODO: Add your command handler code here
	m_wndView.manager.StartGame();
}

void CMainFrame::OnReturn() 
{
	// TODO: Add your command handler code here
	m_wndView.manager.Return();
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_wndView.manager.EndGame();	
//	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateEasy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetRadio(m_wndView.manager.GetDiffculty() == CMapManager::EASY);
}

void CMainFrame::OnEasy() 
{
	// TODO: Add your command handler code here
	m_wndView.manager.SetDiffculty(CMapManager::EASY);
}

void CMainFrame::OnMiddle() 
{
	// TODO: Add your command handler code here
	m_wndView.manager.SetDiffculty(CMapManager::MIDDLE);
}

void CMainFrame::OnUpdateMiddle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetRadio(m_wndView.manager.GetDiffculty() == CMapManager::MIDDLE);
}

void CMainFrame::OnHard() 
{
	// TODO: Add your command handler code here
	m_wndView.manager.SetDiffculty(CMapManager::HARD);
}

void CMainFrame::OnUpdateHard(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetRadio(m_wndView.manager.GetDiffculty() == CMapManager::HARD);
}

// void CMainFrame::OnAppAbout() 
// {
// 	// TODO: Add your command handler code here
// 	CAboutDlg 
// }
