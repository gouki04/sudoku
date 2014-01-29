// GameView.cpp : implementation of the CGameView class
//

#include "stdafx.h"
#include "Sudo.h"
#include "GameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameView

CGameView::CGameView()
{
}

CGameView::~CGameView()
{
}


BEGIN_MESSAGE_MAP(CGameView,CWnd )
	//{{AFX_MSG_MAP(CGameView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameView message handlers

BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CGameView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	manager.Draw(&dc);
	manager.Show(&dc);
	// Do not call CWnd::OnPaint() for painting messages
}


int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	manager.Create(this, 288, 288);
	return 0;
}

void CGameView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	manager.OnMouseMove(point);	
	manager.Process(CMapManager::ONMOUSEMOVE, (void*)&point);
//	CWnd ::OnMouseMove(nFlags, point);
}

void CGameView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	manager.OnLButtonDown(point);
	manager.Process(CMapManager::ONLBUTTONDOWN, (void*)&point);
//	CWnd ::OnLButtonDown(nFlags, point);
}

void CGameView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
//	manager.OnChar(nChar);	
	manager.Process(CMapManager::ONCHAR, (void*)&nChar);
//	CWnd ::OnChar(nChar, nRepCnt, nFlags);
}

void CGameView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	manager.OnRButtonDown(point);
	manager.Process(CMapManager::ONRBUTTONDOWN, (void*)&point);
//	CWnd ::OnRButtonDown(nFlags, point);
}

void CGameView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	manager.Process(CMapManager::ONLBUTTONUP, (void*)&point);	
//	CWnd ::OnLButtonUp(nFlags, point);
}

void CGameView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	manager.Process(CMapManager::ONRBUTTONDOWN, (void*)&point);
//	CWnd ::OnLButtonDblClk(nFlags, point);
}
