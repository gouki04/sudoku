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
// Parameter: CWnd *wnd    // 取得主窗口的指针，主要用于得到主窗口句柄
// Parameter: int width    // view类宽度，固定为640
// Parameter: int height   // view类高度，固定为480
// Description: 程序的主入口函数，载入主要数据并初始化
//************************************
BOOL CMapManager::Create(CWnd *wnd, int width, int height)
{
	Window = wnd;
	MapRect.SetRect(0, 0, width, height);

	/* 载入必要的图片 */
	/* 先不载入底图，因为不同状态的底图不一样 */
	if (!View.Create(width, height, 24)
		|| !Number.LoadBmp("image\\Number2.bmp")
		|| !Cursor.LoadBmp("image\\Cursor2.bmp"))
	{
		return FALSE;
	}
	
	/* 初始化基本数据 */
	CursorPos = 0;
	topPos    = 0 * 96;
	belowPos  = 1 * 96;
	status    = -1;
	diffculty = EASY;
	
	/* 开始主菜单 */
	StartMainMenu();
	return TRUE;
}

//************************************
// Method:    DrawMap
// FullName:  CMapManager::DrawMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: 绘制游戏时的底图
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
// Description: 绘制光标
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
		/* 绘制选定的前景和背景对应的方块上的光标 */
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
// Parameter: int x                 // 横坐标（格子坐标）
// Parameter: int y                 // 纵坐标（格子坐标）
// Parameter: BOOL drawNumber       // 是否重绘数字（默认为真）
// Description: 绘制指定格的底图，并设定是否重绘其上的数字
//************************************
void CMapManager::DrawMap(int x, int y, BOOL drawNumber /*= TRUE*/)
{
	CPoint point(x * GRID_WIDTH, y * GRID_HEIGHT);
	if(status == GAME)
	{	
		if (Map[y][x].isOrigin)
		{
			/* 题目中给定的数字填充背景方块 */
			MBgBelow.SetDrawPos(point);
			MBgBelow.SetSrcPos(CPoint(belowPos + x % 3 * GRID_WIDTH, y % 3 * GRID_HEIGHT));
			MBgBelow.Draw(View);
		}
		else
		{
			/* 玩家写上的数字填充前景方块 */
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
// Description: 绘制所有数字
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
// Parameter: int x      // 横坐标（格子坐标）
// Parameter: int y      // 纵坐标（格子坐标）
// Description: 绘制指定位置的数字
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
// Description: 根据难度从文档中随机读取数独题目
//************************************
void CMapManager::LoadMap()
{
	/* 打开题目文件 */
	CFile file;
	file.Open("MapData.txt", CFile::modeRead);
	
	/* 初始化随机种子 */
	srand((unsigned)time(0));

	/* 根据难度来偏移 */
	/* 81代表一题的81个数，8代表每一题前的号码"#000# "加上一回车换行符共8个字符 */
	if (diffculty == MIDDLE)
	{
		/* 中等难度偏移EASY个 */
		file.Seek(sizeof(char) * EASY * (81 + 8), CFile::current);
	}
	else if (diffculty == HARD)
	{
		/* 困难难度偏移(EASY + MIDDLE)个 */
		file.Seek(sizeof(char) * (EASY + MIDDLE) * (81 + 8), CFile::current);
	}

	/* 随机得到题号（这是在偏移基础上的题号） */
	int No = rand() % diffculty;
	/* 根据题号偏移 */
	/* 6代表题目前的号码"#000# "6个字符 */
	file.Seek(sizeof(char) * No * (81 + 8) + 6, CFile::current);
 	
	/* 读入题目 */
	char txt[81];
	file.Read(&txt, sizeof(char) * 81);
	
	/* 根据题目初始化Map */
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
 			Map[i][j].num = txt[i * 9 + j] - '0';
			Map[i][j].isOrigin = (Map[i][j].num == 0? FALSE : TRUE);
		}
	}

	/* 关闭文件 */
	file.Close();
}

//************************************
// Method:    InitMap
// FullName:  CMapManager::InitMap
// Access:    public 
// Returns:   void
// Qualifier:
// Description: 根据题目组装整个底图
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
// Description: 检查是否完成题目（不管是否正确）
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
// Parameter: int row      // 指定的行号[0,8]
// Description: 检查某一行的数据是否正确
//************************************
BOOL CMapManager::CheckRow(int row)
{
	/* 这里要说明一下 */
	/* 如果这一行是正确的，那它必然包括123456789，9个数字 */
	/* 这里采用位与运算检查 */
	/* 其中check = 0000 0001 1111 1111,后9位为1 */
	/* 假设当前位置的数字为3，则tmp = 1 << 2 = 0000 0000 0000 0100 */
	/* 则check &= ~tmp = */
	/* 0000 0001 1111 1111 &  */
	/* 1111 1111 1111 1011 =  */
	/* 0000 0001 1111 1011,则第3位变为0，这样只有当9个数刚好是1-9的时候，check才为0 */

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
// Parameter: int col      // 指定的列号[0,8]
// Description: 检查某一列的数据是否正确
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
// Parameter: int grid       // 指定的九宫格号[0,8]
// Description: 检查某一个九宫格的数据是否正确
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
// Description: 检查答案是否正确
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
// Parameter: CDC* dc      // 设备上下文句柄
// Description: 主显示函数，根据状态显示
//************************************
void CMapManager::Show(CDC* dc)
{
	/* 以下的绘制函数不会重绘整个底图 */
	/* 只绘制需重绘的地方 */
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
// Description: 主界面状态的显示函数
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
// Description: 游戏状态的显示函数
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
// Description: 选项状态的显示函数
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
// Parameter: CALLBACKTYPE type       // 消息类型
// Parameter: void *data              // 消息数据（void*能转换为任何类型）
// Description: 主处理器，根据状态处理
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
// Parameter: CALLBACKTYPE type   // 消息类型
// Parameter: void *data          // 消息数据（void*能转换为任何类型）
// Description: 游戏状态的处理器
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
// Parameter: CALLBACKTYPE type     // 消息类型
// Parameter: void *data            // 消息数据（void*能转换为任何类型）
// Description: 主界面状态的处理器
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
// Parameter: CALLBACKTYPE type      // 消息类型
// Parameter: void *data             // 消息数据（void*能转换为任何类型）
// Description: 选项状态的处理器
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
// Parameter: CALLBACKTYPE type       // 消息类型
// Parameter: void *data              // 消息数据（void*能转换为任何类型）
// Description: 胜利状态的处理器
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
// Parameter: CPoint point       // 鼠标位置（像素坐标）
// Description: 游戏状态下的鼠标移动事件处理器
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
// Parameter: UINT nChar       // 按键的ASCII码
// Description: 游戏状态下的键盘输入事件处理器
//************************************
void CMapManager::GameChar(UINT nChar)
{
	int x = CursorPos.x;
	int y = CursorPos.y;
	
	/* 不能修改题目给定的数字 */
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
	/* ESC键返回主界面 */
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
			AfxMessageBox("你做错了! @_@!");
		}
	}
}

//************************************
// Method:    MenuLButtonDown
// FullName:  CMapManager::MenuLButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CPoint point      // 鼠标位置（像素坐标）
// Description: 主界面状态下的单击事件处理器
//************************************
void CMapManager::MenuLButtonDown(CPoint point)
{	
	CRect ButtonRect;
	ButtonRect.SetRect(IndexToPoint(5, 4), IndexToPoint(7, 7));
	if (ButtonRect.PtInRect(point))
	{
		/* 这里本来要绘制鼠标按下效果，但程序速度太快，根本显示不了 */
		DrawMap(5, CursorPos.y);
	 	MCursor.SetSrcPos(96, 0);
	 	MCursor.Draw(View);
		InvalidateRect(Window->m_hWnd, MapRect, FALSE);
		
		/* 开始游戏按钮 */
		if ((CRect(IndexToPoint(5, 4), IndexToPoint(7, 5))).PtInRect(point))
		{	
 			StartGame();
		}

		/* 选项按钮 */
		if ((CRect(IndexToPoint(5, 5), IndexToPoint(7, 6))).PtInRect(point))
		{
			StartOption();
		}

		/* 结束游戏按钮 */
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
// Parameter: CPoint point       // 鼠标位置（像素坐标）
// Description: 主界面状态下的鼠标移动事件处理器
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
// Parameter: CPoint point     // 鼠标位置（像素坐标）
// Description: 主界面状态下的左键弹起事件处理器
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
// Parameter: CPoint point     // 鼠标位置（像素坐标）
// Description: 选项状态下的鼠标移动事件处理器
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
// Parameter: CPoint point      // 鼠标位置（像素坐标）
// Description: 选项状态下的单击事件处理器
//************************************
void CMapManager::OptionLButtonDown(CPoint point)
{
	int index;
	/* 前景方块 */
	if (CRect(IndexToPoint(3, 1), IndexToPoint(8, 3)).PtInRect(point))
	{
		index = topPos / 96;
		DrawMap(3 + index % 5, (1 + index / 5));
	
		topPos = ((point.y / GRID_HEIGHT - 1) * 5 + (point.x / GRID_WIDTH - 3)) * 96;
	}
	/* 背景方块 */
	else if (CRect(IndexToPoint(3, 4), IndexToPoint(8, 6)).PtInRect(point))
	{
		index = belowPos / 96;
		DrawMap(3 + index % 5, (4 + index / 5));

		belowPos = ((point.y / GRID_HEIGHT - 4) * 5 + (point.x / GRID_WIDTH - 3)) * 96;
	}
	/* 返回按钮 */
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
// Parameter: CPoint point      // 鼠标位置（像素坐标）
// Description: 胜利状态下的单击事件处理器
//************************************
void CMapManager::WinLButtonDown(CPoint point)
{
	/* 继续按钮 */
	if (CRect(IndexToPoint(0, 8), IndexToPoint(3, 9)).PtInRect(point))
	{
		StartGame();
	}
	/* 返回按钮 */
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
// Description: 开始游戏（转为游戏状态）
//************************************
void CMapManager::StartGame()
{
	/* 开始游戏 */
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
	/* 重开游戏 */
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
// Description: 开始主界面（转为主界面状态）
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
// Description: 开始选项（转为选项状态）
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
// Description: 开始胜利（转为胜利状态）
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
// Description: 结束游戏
//************************************
void CMapManager::EndGame()
{
	if (AfxMessageBox("离开游戏吗？", MB_YESNO) == IDYES)
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
// Description: 返回到主界面
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
// Description: 设定游戏难度
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
// Description: 取得游戏当前难度
//************************************
int CMapManager::GetDiffculty()
{
	return diffculty;
}
