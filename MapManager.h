// MapManager.h: interface for the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMANAGER_H__330F4346_02C5_4EF1_A397_176590884C01__INCLUDED_)
#define AFX_MAPMANAGER_H__330F4346_02C5_4EF1_A397_176590884C01__INCLUDED_

#include "Sprite.h"	// Added by ClassView
#include "DibSection.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CMapManager  
{
public:
	/************************************************************************/
	/* 主要状态                                                             */
	
	enum
	{
		MAINMENU = 1,     // 主界面
		GAME     = 2,     // 游戏
		OPTION   = 3,     // 选项
		WIN      = 4,     // 胜利
	};
	
	/* 回调的类型 */
	enum CALLBACKTYPE
	{
		ONLBUTTONDOWN = 1,    // 鼠标按下事件
		ONLBUTTONUP   = 2,    // 鼠标弹起事件
		ONRBUTTONDOWN = 3,    // 鼠标右键按下事件
		ONMOUSEMOVE   = 4,    // 鼠标移动事件
		ONCHAR        = 5,    // 键盘按下事件
	};

	/* 难度 */
	enum 
	{
		EASY   = 12,    // 简单题目数
		MIDDLE = 43,    // 中等题目数
		HARD   = 46,    // 困难题目数
		SUM    = 101,   // 总数
	};
	/************************************************************************/
	
public:
	/* 构造/析构函数*/
	CMapManager();
	virtual ~CMapManager();

	/* 程序的入口，初始化一些基本的数据 */
	BOOL Create(CWnd *wnd, int width, int height);

	/* 载入/初始化数独题目的地图 */
	void LoadMap();
	void InitMap();

	/* 状态转换器 */
	void StartGame();
	void StartMainMenu();
	void StartOption();
	void StartWin();
	void EndGame();
	void Return();

	/* 显示函数 */
	void Show(CDC* dc);	
	void ShowMainMenu();
	void ShowGame();
	void ShowOption();
	
	/* 主处理函数 */
	void Process(CALLBACKTYPE type, void *data);
	void ProcessMenu(CALLBACKTYPE type, void *data);
	void ProcessGame(CALLBACKTYPE type, void* data);
	void ProcessOption(CALLBACKTYPE type, void *data);
	void ProcessWin(CALLBACKTYPE type, void *data);
	
	/* 游戏状态的处理器 */
	void GameMouseMove(CPoint point);
	void GameChar(UINT nChar);
	
	/* 主界面状态的处理器 */
	void MenuLButtonUp(CPoint point);
	void MenuMouseMove(CPoint point);
	void MenuLButtonDown(CPoint point);
	
	/* 选项状态的处理器 */
	void OptionLButtonDown(CPoint point);
	void OptionMouseMove(CPoint point);
	
	/* 胜利状态的处理器 */
	void WinLButtonDown(CPoint point);
	
	/* 基本绘制函数 */
	void DrawMap(int x, int y, BOOL drawNumber = TRUE);
	void DrawCursor();
	void DrawMap();
	void DrawNumber(int x, int y);
	void DrawNumber();
	
	/* 检查函数 */
	BOOL CheckGrid(int grid);
	BOOL CheckCol(int col);
	BOOL CheckRow(int row);
	BOOL CheckFinish();
	BOOL CheckSuccess();

	/* 取得/更新难度 */
	int GetDiffculty();
	void SetDiffculty(int dif);

protected:
	CDibSection BG;     // 底图
	CDibSection View;   // 所有绘制都合成在本图
	CDibSection Cursor; // 光标图
	CDibSection Number; // 数字图

	CSprite MCursor;   // 光标贴图
	CSprite MBgTop;    // 前景贴图
	CSprite MBgBelow;  // 背景贴图
		
	UINT topPos;          // 前景贴图的号码
	UINT belowPos;        // 背景贴图的号码
	CPoint CursorPos;     // 鼠标位置（格子坐标）
	CRect MapRect;        // 底图区域
	int status;           // 游戏状态
	int diffculty;        // 游戏难度
	CWnd* Window;         // 主窗口指针

	struct MapData{
		int num;        // 数字
		BOOL isOrigin;  // 是否是题目本身的数字
	};

	MapData Map[9][9];	   // 记录解答过程中的数据
};

#endif // !defined(AFX_MAPMANAGER_H__330F4346_02C5_4EF1_A397_176590884C01__INCLUDED_)
