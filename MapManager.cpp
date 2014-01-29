// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sudo.h"
#include "MapManager.h"
#include "coordinate.h"
#include <time.h>
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapManager::CMapManager()
{

}

CMapManager::~CMapManager()
{

}

//************************************
// Method:    Create
// FullName:  CMapManager::Create
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: CWnd *wnd    // ȡ�������ڵ�ָ�룬��Ҫ���ڵõ������ھ��
// Parameter: int width    // view���ȣ��̶�Ϊ640
// Parameter: int height   // view��߶ȣ��̶�Ϊ480
// Description: ���������ں�����������Ҫ���ݲ���ʼ��
//************************************
BOOL CMapManager::Create(CWnd *wnd, int width, int height)
{
	Window = wnd;
	MapRect.SetRect(0, 0, width, height);

	/* �����Ҫ��ͼƬ */
	/* �Ȳ������ͼ����Ϊ��ͬ״̬�ĵ�ͼ��һ�� */
	if (!View.Create(width, height, 24)
		|| !Number.LoadBmp("image\\Number2.bmp")
		|| !Cursor.LoadBmp("image\\Cursor2.bmp"))
	{
		return FALSE;
	}
	
	/* ��ʼ���������� */
	CursorPos = 0;
	topPos    = 0 * 96;
	belowPos  = 1 * 96;
	status    = -1;
	diffculty = EASY;
	
	/* ��ʼ���˵� */
	StartMainMenu();
	return TRUE;
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������Ϸʱ�ĵ�ͼ
//************************************
void CMapManager::DrawMap()
{
	InitMap();
	DrawNumber();
}

//************************************
// Method:    DrawCursor
// FullName:  CMapManager::DrawCursor
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ���ƹ��
//************************************
void CMapManager::DrawCursor()
{
	CPoint point(CursorPos.x * GRID_WIDTH, CursorPos.y * GRID_HEIGHT);
	if(status == GAME)
	{
		MCursor.SetDrawPos(point + CPoint(2, 2));
	}
	else if(status == MAINMENU)
	{
		MCursor.SetDrawPos(CPoint(5 * GRID_WIDTH, point.y));
	}
	else if(status == OPTION)
	{
		/* ����ѡ����ǰ���ͱ�����Ӧ�ķ����ϵĹ�� */
		int index = topPos / 96;
		CPoint LT(3  * GRID_WIDTH, (1 + index / 5) * GRID_HEIGHT);
		MCursor.SetDrawPos(LT + CPoint((index % 5) * GRID_WIDTH, 0));
		MCursor.Draw(View);

		index = belowPos / 96;
		LT.y = (4 + index / 5) * GRID_HEIGHT;
		MCursor.SetDrawPos(LT + CPoint((index % 5) * GRID_WIDTH, 0));
		MCursor.Draw(View);

		MCursor.SetDrawPos(point);
	}

	MCursor.Draw(View);
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x                 // �����꣨�������꣩
// Parameter: int y                 // �����꣨�������꣩
// Parameter: BOOL drawNumber       // �Ƿ��ػ����֣�Ĭ��Ϊ�棩
// Description: ����ָ����ĵ�ͼ�����趨�Ƿ��ػ����ϵ�����
//************************************
void CMapManager::DrawMap(int x, int y, BOOL drawNumber /*= TRUE*/)
{
	CPoint point(x * GRID_WIDTH, y * GRID_HEIGHT);
	if(status == GAME)
	{	
		if (Map[y][x].isOrigin)
		{
			/* ��Ŀ�и�����������䱳������ */
			MBgBelow.SetDrawPos(point);
			MBgBelow.SetSrcPos(CPoint(belowPos + x % 3 * GRID_WIDTH, y % 3 * GRID_HEIGHT));
			MBgBelow.Draw(View);
		}
		else
		{
			/* ���д�ϵ��������ǰ������ */
			MBgTop.SetDrawPos(point);
			MBgTop.SetSrcPos(CPoint(topPos + x % 3 * GRID_WIDTH, y % 3 * GRID_HEIGHT));
			MBgTop.Draw(View);
		}
		if (drawNumber)
		{
			DrawNumber(x, y);
		}
	}	
	else if(status == MAINMENU)
	{
		View.Copy(BG, point, CSize(64, 32), point);
	}
	else if(status == OPTION)
	{
		View.Copy(BG, point, CSize(32, 32), point);
	}
}

//************************************
// Method:    DrawNumber
// FullName:  CMapManager::DrawNumber
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������������
//************************************
void CMapManager::DrawNumber()
{
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			if(Map[i][j].num != 0)
			{
				DrawNumber(j, i);
			}
		}
	}
}

//************************************
// Method:    DrawNumber
// FullName:  CMapManager::DrawNumber
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int x      // �����꣨�������꣩
// Parameter: int y      // �����꣨�������꣩
// Description: ����ָ��λ�õ�����
//************************************
void CMapManager::DrawNumber(int x, int y)
{
	CPoint point(x * GRID_WIDTH, y * GRID_HEIGHT);
	if(Map[y][x].num > 0 && Map[y][x].num < 10)
	{
		View.Mix(Number, point, CSize(GRID_WIDTH, GRID_HEIGHT), 
			CPoint((Map[y][x].num) * GRID_WIDTH));
	}
}

//************************************
// Method:    LoadMap
// FullName:  CMapManager::LoadMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: �����Ѷȴ��ĵ��������ȡ������Ŀ
//************************************
void CMapManager::LoadMap()
{
	/* ����Ŀ�ļ� */
	CFile file;
	file.Open("MapData.txt", CFile::modeRead);
	
	/* ��ʼ��������� */
	srand((unsigned)time(0));

	/* �����Ѷ���ƫ�� */
	/* 81����һ���81������8����ÿһ��ǰ�ĺ���"#000# "����һ�س����з���8���ַ� */
	if (diffculty == MIDDLE)
	{
		/* �е��Ѷ�ƫ��EASY�� */
		file.Seek(sizeof(char) * EASY * (81 + 8), CFile::current);
	}
	else if (diffculty == HARD)
	{
		/* �����Ѷ�ƫ��(EASY + MIDDLE)�� */
		file.Seek(sizeof(char) * (EASY + MIDDLE) * (81 + 8), CFile::current);
	}

	/* ����õ���ţ�������ƫ�ƻ����ϵ���ţ� */
	int No = rand() % diffculty;
	/* �������ƫ�� */
	/* 6������Ŀǰ�ĺ���"#000# "6���ַ� */
	file.Seek(sizeof(char) * No * (81 + 8) + 6, CFile::current);
 	
	/* ������Ŀ */
	char txt[81];
	file.Read(&txt, sizeof(char) * 81);
	
	/* ������Ŀ��ʼ��Map */
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
 			Map[i][j].num = txt[i * 9 + j] - '0';
			Map[i][j].isOrigin = (Map[i][j].num == 0? FALSE : TRUE);
		}
	}

	/* �ر��ļ� */
	file.Close();
}

//************************************
// Method:    InitMap
// FullName:  CMapManager::InitMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������Ŀ��װ������ͼ
//************************************
void CMapManager::InitMap()
{
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			if(Map[i][j].isOrigin == TRUE)
			{
				MBgBelow.SetDrawPos(IndexToPoint(j, i));
				MBgBelow.SetSrcPos(CPoint(belowPos + j % 3 * GRID_WIDTH, 
					i % 3 * GRID_HEIGHT));
				MBgBelow.Draw(View);
			}
			else
			{
				MBgTop.SetDrawPos(IndexToPoint(j, i));
				MBgTop.SetSrcPos(CPoint(topPos + j % 3 * GRID_WIDTH, 
					i % 3 * GRID_HEIGHT));
				MBgTop.Draw(View);
			}
		}
	}
}

//************************************
// Method:    CheckFinish
// FullName:  CMapManager::CheckFinish
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Description: ����Ƿ������Ŀ�������Ƿ���ȷ��
//************************************
BOOL CMapManager::CheckFinish()
{
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			if (Map[i][j].num == 0)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

//************************************
// Method:    CheckRow
// FullName:  CMapManager::CheckRow
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int row      // ָ�����к�[0,8]
// Description: ���ĳһ�е������Ƿ���ȷ
//************************************
BOOL CMapManager::CheckRow(int row)
{
	/* ����Ҫ˵��һ�� */
	/* �����һ������ȷ�ģ�������Ȼ����123456789��9������ */
	/* �������λ�������� */
	/* ����check = 0000 0001 1111 1111,��9λΪ1 */
	/* ���赱ǰλ�õ�����Ϊ3����tmp = 1 << 2 = 0000 0000 0000 0100 */
	/* ��check &= ~tmp = */
	/* 0000 0001 1111 1111 &  */
	/* 1111 1111 1111 1011 =  */
	/* 0000 0001 1111 1011,���3λ��Ϊ0������ֻ�е�9�����պ���1-9��ʱ��check��Ϊ0 */

	DWORD check = 0x01FF;
	DWORD tmp = 0;
	for (int i=0; i<9; i++)
	{
		tmp = 1 << (Map[row][i].num - 1);
		check &= ~tmp;
	}
	return check;
}

//************************************
// Method:    CheckCol
// FullName:  CMapManager::CheckCol
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int col      // ָ�����к�[0,8]
// Description: ���ĳһ�е������Ƿ���ȷ
//************************************
BOOL CMapManager::CheckCol(int col)
{
	DWORD check = 0x01FF;
	DWORD tmp = 0;
	for (int i=0; i<9; i++)
	{
		tmp = 1 << (Map[i][col].num - 1);
		check &= ~tmp;
	}
	return check;
}

//************************************
// Method:    CheckGrid
// FullName:  CMapManager::CheckGrid
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: int grid       // ָ���ľŹ����[0,8]
// Description: ���ĳһ���Ź���������Ƿ���ȷ
//************************************
BOOL CMapManager::CheckGrid(int grid)
{
	DWORD check = 0x01FF;
	DWORD tmp = 0;
	int top = grid / 3 * 3;
	int left = (grid % 3) * 3;
	for (int i=top; i<top+3; i++)
	{
		for (int j=left; j<left+3; j++)
		{
			tmp = 1 << (Map[i][j].num - 1);
			check &= ~tmp;
		}
	}
	return check;
}

//************************************
// Method:    CheckSuccess
// FullName:  CMapManager::CheckSuccess
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Description: �����Ƿ���ȷ
//************************************
BOOL CMapManager::CheckSuccess()
{
	int i;
	for (i=0; i<9; i++)
	{
		if(CheckRow(i) != 0 || CheckCol(i) != 0 || CheckGrid(i) != 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//************************************
// Method:    Show
// FullName:  CMapManager::Show
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CDC* dc      // �豸�����ľ��
// Description: ����ʾ����������״̬��ʾ
//************************************
void CMapManager::Show(CDC* dc)
{
	/* ���µĻ��ƺ��������ػ�������ͼ */
	/* ֻ�������ػ�ĵط� */
	switch(status)
	{
	case MAINMENU:
		ShowMainMenu();
		break;
	case GAME:
		ShowGame();
	    break;
	case OPTION:
		ShowOption();
		break;
	default:
	    break;
	}
	if (View.IsOK())
	{
		View.Draw(*dc, 0, 0, View.Width(), View.Height());
	}
}

//************************************
// Method:    ShowMainMenu
// FullName:  CMapManager::ShowMainMenu
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������״̬����ʾ����
//************************************
void CMapManager::ShowMainMenu()
{
	CRect ButtonRect;
	ButtonRect.SetRect(IndexToPoint(5, 4), IndexToPoint(7, 7));
	if (ButtonRect.PtInRect(IndexToPoint(CursorPos)))
	{
		DrawCursor();
	}
}

//************************************
// Method:    ShowGame
// FullName:  CMapManager::ShowGame
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��Ϸ״̬����ʾ����
//************************************
void CMapManager::ShowGame()
{	
	if(MapRect.PtInRect(IndexToPoint(CursorPos)))
	{
		DrawMap(CursorPos.x, CursorPos.y, FALSE);
		DrawNumber(CursorPos.x, CursorPos.y);
		DrawCursor();
	}
}

//************************************
// Method:    ShowOption
// FullName:  CMapManager::ShowOption
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ѡ��״̬����ʾ����
//************************************
void CMapManager::ShowOption()
{
	CRect optionRect1;
	CRect optionRect2;
	optionRect1.SetRect(IndexToPoint(3, 1), IndexToPoint(8, 3));
	optionRect2.SetRect(IndexToPoint(3, 4), IndexToPoint(8, 6));
	if (optionRect1.PtInRect(IndexToPoint(CursorPos)) || 
		optionRect2.PtInRect(IndexToPoint(CursorPos)))
	{
		DrawCursor();
	}
}

//************************************
// Method:    Process
// FullName:  CMapManager::Process
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CALLBACKTYPE type       // ��Ϣ����
// Parameter: void *data              // ��Ϣ���ݣ�void*��ת��Ϊ�κ����ͣ�
// Description: ��������������״̬����
//************************************
void CMapManager::Process(CALLBACKTYPE type, void *data)
{
	switch(status)
	{
	case MAINMENU:
		ProcessMenu(type, data);
		break;
	case GAME:
		ProcessGame(type, data);
		break;
	case OPTION:
		ProcessOption(type, data);
		break;
	case WIN:
		ProcessWin(type, data);
		break;
	default:
	    break;
	}
	InvalidateRect(Window->m_hWnd, MapRect, FALSE);
}

//************************************
// Method:    ProcessGame
// FullName:  CMapManager::ProcessGame
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CALLBACKTYPE type   // ��Ϣ����
// Parameter: void *data          // ��Ϣ���ݣ�void*��ת��Ϊ�κ����ͣ�
// Description: ��Ϸ״̬�Ĵ�����
//************************************
void CMapManager::ProcessGame(CALLBACKTYPE type, void *data)
{
	switch(type)
	{
	case ONMOUSEMOVE:
		GameMouseMove(*(CPoint*)data);
	    break;
	case ONCHAR:
		GameChar(*(UINT*)data);
	    break;
	default:
	    break;
	}
}

//************************************
// Method:    ProcessMenu
// FullName:  CMapManager::ProcessMenu
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CALLBACKTYPE type     // ��Ϣ����
// Parameter: void *data            // ��Ϣ���ݣ�void*��ת��Ϊ�κ����ͣ�
// Description: ������״̬�Ĵ�����
//************************************
void CMapManager::ProcessMenu(CALLBACKTYPE type, void *data)
{
	switch(type)
	{
	case ONLBUTTONDOWN:
		MenuLButtonDown(*(CPoint*)data);
		break;
	case ONMOUSEMOVE:
		MenuMouseMove(*(CPoint*)data);
		break;
	case ONLBUTTONUP:
		MenuLButtonUp(*(CPoint*)data);
		break;
	default:
	    break;
	}
}

//************************************
// Method:    ProcessOption
// FullName:  CMapManager::ProcessOption
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CALLBACKTYPE type      // ��Ϣ����
// Parameter: void *data             // ��Ϣ���ݣ�void*��ת��Ϊ�κ����ͣ�
// Description: ѡ��״̬�Ĵ�����
//************************************
void CMapManager::ProcessOption(CALLBACKTYPE type, void *data)
{
	switch(type)
	{
	case ONMOUSEMOVE:
		OptionMouseMove(*(CPoint*)data);
		break;
	case ONLBUTTONDOWN:
		OptionLButtonDown(*(CPoint*)data);
	    break;
	default:
	    break;
	}
}

//************************************
// Method:    ProcessWin
// FullName:  CMapManager::ProcessWin
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CALLBACKTYPE type       // ��Ϣ����
// Parameter: void *data              // ��Ϣ���ݣ�void*��ת��Ϊ�κ����ͣ�
// Description: ʤ��״̬�Ĵ�����
//************************************
void CMapManager::ProcessWin(CALLBACKTYPE type, void *data)
{
	switch(type)
	{
	case ONLBUTTONDOWN:
		WinLButtonDown(*(CPoint*)data);
	    break;
	default:
	    break;
	}
}

//************************************
// Method:    GameMouseMove
// FullName:  CMapManager::GameMouseMove
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point       // ���λ�ã��������꣩
// Description: ��Ϸ״̬�µ�����ƶ��¼�������
//************************************
void CMapManager::GameMouseMove(CPoint point)
{
	if(MapRect.PtInRect(point))
	{
		CPoint Pos = PointToIndex(point);
		if (CursorPos != Pos) 
		{
			DrawMap(CursorPos.x, CursorPos.y);
			CursorPos = Pos;	
		}
	}
}

//************************************
// Method:    GameChar
// FullName:  CMapManager::GameChar
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: UINT nChar       // ������ASCII��
// Description: ��Ϸ״̬�µļ��������¼�������
//************************************
void CMapManager::GameChar(UINT nChar)
{
	int x = CursorPos.x;
	int y = CursorPos.y;
	
	/* �����޸���Ŀ���������� */
	if (Map[y][x].isOrigin)
	{
		return;
	}
	
	switch(nChar)
	{
	case '0':
		Map[y][x].num = 0;
		break;
	case '1':
		Map[y][x].num = 1;
		break;
	case '2':
		Map[y][x].num = 2;
		break;
	case '3':
		Map[y][x].num = 3;
	    break;
	case '4':
		Map[y][x].num = 4;
	    break;
	case '5':
		Map[y][x].num = 5;
		break;
	case '6':
		Map[y][x].num = 6;
		break;
	case '7':
		Map[y][x].num = 7;
	    break;
	case '8':
		Map[y][x].num = 8;
	    break;
	case '9':
		Map[y][x].num = 9;
	    break;
	/* ESC������������ */
	case VK_ESCAPE:
		Return();
		break;
	default:
	    break;
	}

	if (CheckFinish())
	{
		if(CheckSuccess())
		{
			StartWin();
		}
		else
		{
			AfxMessageBox("��������! @_@!");
		}
	}
}

//************************************
// Method:    MenuLButtonDown
// FullName:  CMapManager::MenuLButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point      // ���λ�ã��������꣩
// Description: ������״̬�µĵ����¼�������
//************************************
void CMapManager::MenuLButtonDown(CPoint point)
{	
	CRect ButtonRect;
	ButtonRect.SetRect(IndexToPoint(5, 4), IndexToPoint(7, 7));
	if (ButtonRect.PtInRect(point))
	{
		/* ���ﱾ��Ҫ������갴��Ч�����������ٶ�̫�죬������ʾ���� */
		DrawMap(5, CursorPos.y);
	 	MCursor.SetSrcPos(96, 0);
	 	MCursor.Draw(View);
		InvalidateRect(Window->m_hWnd, MapRect, FALSE);
		
		/* ��ʼ��Ϸ��ť */
		if ((CRect(IndexToPoint(5, 4), IndexToPoint(7, 5))).PtInRect(point))
		{	
 			StartGame();
		}

		/* ѡ�ť */
		if ((CRect(IndexToPoint(5, 5), IndexToPoint(7, 6))).PtInRect(point))
		{
			StartOption();
		}

		/* ������Ϸ��ť */
		if ((CRect(IndexToPoint(5, 6), IndexToPoint(7, 7))).PtInRect(point))
		{
 			EndGame();
			MenuLButtonUp(point);
		}
	}	
}

//************************************
// Method:    MenuMouseMove
// FullName:  CMapManager::MenuMouseMove
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point       // ���λ�ã��������꣩
// Description: ������״̬�µ�����ƶ��¼�������
//************************************
void CMapManager::MenuMouseMove(CPoint point)
{
	CRect ButtonRect;
	ButtonRect.SetRect(IndexToPoint(5, 4), IndexToPoint(7, 7));
	if (ButtonRect.PtInRect(point))
	{
		DrawMap(5, CursorPos.y);
		CursorPos = PointToIndex(point);
	}
}

//************************************
// Method:    MenuLButtonUp
// FullName:  CMapManager::MenuLButtonUp
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point     // ���λ�ã��������꣩
// Description: ������״̬�µ���������¼�������
//************************************
void CMapManager::MenuLButtonUp(CPoint point)
{
	CRect ButtonRect;
	ButtonRect.SetRect(IndexToPoint(5, 4), IndexToPoint(7, 7));
	if (ButtonRect.PtInRect(point))
	{
		DrawMap(5, CursorPos.y);
		MCursor.SetSrcPos(32, 0);
		MCursor.Draw(View);
	}
}

//************************************
// Method:    OptionMouseMove
// FullName:  CMapManager::OptionMouseMove
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point     // ���λ�ã��������꣩
// Description: ѡ��״̬�µ�����ƶ��¼�������
//************************************
void CMapManager::OptionMouseMove(CPoint point)
{
	if (CRect(IndexToPoint(3, 1), IndexToPoint(8, 3)).PtInRect(point) || 
		CRect(IndexToPoint(3, 4), IndexToPoint(6, 6)).PtInRect(point))
	{
		DrawMap(CursorPos.x, CursorPos.y);
		CursorPos = PointToIndex(point);
	}
}

//************************************
// Method:    OptionLButtonDown
// FullName:  CMapManager::OptionLButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point      // ���λ�ã��������꣩
// Description: ѡ��״̬�µĵ����¼�������
//************************************
void CMapManager::OptionLButtonDown(CPoint point)
{
	int index;
	/* ǰ������ */
	if (CRect(IndexToPoint(3, 1), IndexToPoint(8, 3)).PtInRect(point))
	{
		index = topPos / 96;
		DrawMap(3 + index % 5, (1 + index / 5));
	
		topPos = ((point.y / GRID_HEIGHT - 1) * 5 + (point.x / GRID_WIDTH - 3)) * 96;
	}
	/* �������� */
	else if (CRect(IndexToPoint(3, 4), IndexToPoint(8, 6)).PtInRect(point))
	{
		index = belowPos / 96;
		DrawMap(3 + index % 5, (4 + index / 5));

		belowPos = ((point.y / GRID_HEIGHT - 4) * 5 + (point.x / GRID_WIDTH - 3)) * 96;
	}
	/* ���ذ�ť */
	else if (CRect(IndexToPoint(6, 7), IndexToPoint(8, 8)).PtInRect(point))
	{
		StartMainMenu();
	}
}

//************************************
// Method:    WinLButtonDown
// FullName:  CMapManager::WinLButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point      // ���λ�ã��������꣩
// Description: ʤ��״̬�µĵ����¼�������
//************************************
void CMapManager::WinLButtonDown(CPoint point)
{
	/* ������ť */
	if (CRect(IndexToPoint(0, 8), IndexToPoint(3, 9)).PtInRect(point))
	{
		StartGame();
	}
	/* ���ذ�ť */
	else if (CRect(IndexToPoint(5, 8), IndexToPoint(8, 9)).PtInRect(point))
	{
		StartMainMenu();
	}
}

//************************************
// Method:    StartGame
// FullName:  CMapManager::StartGame
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼ��Ϸ��תΪ��Ϸ״̬��
//************************************
void CMapManager::StartGame()
{
	/* ��ʼ��Ϸ */
	if(status != GAME)
	{
		if(!BG.LoadBmp("image\\BG.bmp"))
		{
			EndGame();
		}
		status = GAME;
		MBgTop.Set(&BG, CPoint(0, 0), CSize(32, 32), CPoint(0, 0));
		MBgBelow.Set(&BG, CPoint(0, 0), CSize(32, 32), CPoint(96, 0));

		MCursor.SetSrcPos(2, 2);
		MCursor.SetSize(CSize(28, 28));
		LoadMap();
		DrawMap();
	}
	/* �ؿ���Ϸ */
	else if(status == GAME)
	{
		LoadMap();
		DrawMap();
	}
}	

//************************************
// Method:    StartMainMenu
// FullName:  CMapManager::StartMainMenu
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼ�����棨תΪ������״̬��
//************************************
void CMapManager::StartMainMenu()
{
	if(status != MAINMENU)
	{
		if(!BG.LoadBmp("image\\Menu3.bmp"))
		{
			EndGame();
		}
		View.Copy(BG, CPoint(0, 0), CSize(288, 288), CPoint(0, 0));
		MCursor.Set(&Cursor, CPoint(0, 0), CSize(64, 32), CPoint(32, 0));
		status = MAINMENU;
	}
}

//************************************
// Method:    StartOption
// FullName:  CMapManager::StartOption
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼѡ�תΪѡ��״̬��
//************************************
void CMapManager::StartOption()
{
	if(status != OPTION)
	{
		if(!BG.LoadBmp("image\\Option.bmp"))
		{
			EndGame();
		}
		status = OPTION;
		View.Copy(BG, CPoint(0, 0), CSize(288, 288), CPoint(0, 0));		
		MCursor.SetSrcPos(0, 0);
		MCursor.SetSize(CSize(32, 32));

		int index = topPos / 96;
		CPoint LT(3  * GRID_WIDTH, (1 + index / 5) * GRID_HEIGHT);
		MCursor.SetDrawPos(LT + CPoint((index % 5) * GRID_WIDTH, 0));
		MCursor.Draw(View);

		index = belowPos / 96;
		LT.y = (4 + index / 5) * GRID_HEIGHT;
		MCursor.SetDrawPos(LT + CPoint((index % 5) * GRID_WIDTH, 0));
		MCursor.Draw(View);
	}
}

//************************************
// Method:    StartWin
// FullName:  CMapManager::StartWin
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ��ʼʤ����תΪʤ��״̬��
//************************************
void CMapManager::StartWin()
{
	if (status != WIN)
	{	
		if(!BG.LoadBmp("image\\Win.bmp"))
		{
			EndGame();
		}
		status = WIN;
		View.Copy(BG, CPoint(0, 0), CSize(288, 288), CPoint(0, 0));
	}
}

//************************************
// Method:    EndGame
// FullName:  CMapManager::EndGame
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ������Ϸ
//************************************
void CMapManager::EndGame()
{
	if (AfxMessageBox("�뿪��Ϸ��", MB_YESNO) == IDYES)
	{
		PostQuitMessage(0);
	}
}

//************************************
// Method:    Return
// FullName:  CMapManager::Return
// Access:    public 
// Returns:   void
// Qualifier:
// Description: ���ص�������
//************************************
void CMapManager::Return()
{
	StartMainMenu();
}

//************************************
// Method:    SetDiffculty
// FullName:  CMapManager::SetDiffculty
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int dif
// Description: �趨��Ϸ�Ѷ�
//************************************
void CMapManager::SetDiffculty(int dif)
{
	diffculty = dif;
}

//************************************
// Method:    GetDiffculty
// FullName:  CMapManager::GetDiffculty
// Access:    public 
// Returns:   int
// Qualifier:
// Description: ȡ����Ϸ��ǰ�Ѷ�
//************************************
int CMapManager::GetDiffculty()
{
	return diffculty;
}
