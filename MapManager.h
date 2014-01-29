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
	/* ��Ҫ״̬                                                             */
	
	enum
	{
		MAINMENU = 1,     // ������
		GAME     = 2,     // ��Ϸ
		OPTION   = 3,     // ѡ��
		WIN      = 4,     // ʤ��
	};
	
	/* �ص������� */
	enum CALLBACKTYPE
	{
		ONLBUTTONDOWN = 1,    // ��갴���¼�
		ONLBUTTONUP   = 2,    // ��굯���¼�
		ONRBUTTONDOWN = 3,    // ����Ҽ������¼�
		ONMOUSEMOVE   = 4,    // ����ƶ��¼�
		ONCHAR        = 5,    // ���̰����¼�
	};

	/* �Ѷ� */
	enum 
	{
		EASY   = 12,    // ����Ŀ��
		MIDDLE = 43,    // �е���Ŀ��
		HARD   = 46,    // ������Ŀ��
		SUM    = 101,   // ����
	};
	/************************************************************************/
	
public:
	/* ����/��������*/
	CMapManager();
	virtual ~CMapManager();

	/* �������ڣ���ʼ��һЩ���������� */
	BOOL Create(CWnd *wnd, int width, int height);

	/* ����/��ʼ��������Ŀ�ĵ�ͼ */
	void LoadMap();
	void InitMap();

	/* ״̬ת���� */
	void StartGame();
	void StartMainMenu();
	void StartOption();
	void StartWin();
	void EndGame();
	void Return();

	/* ��ʾ���� */
	void Show(CDC* dc);	
	void ShowMainMenu();
	void ShowGame();
	void ShowOption();
	
	/* �������� */
	void Process(CALLBACKTYPE type, void *data);
	void ProcessMenu(CALLBACKTYPE type, void *data);
	void ProcessGame(CALLBACKTYPE type, void* data);
	void ProcessOption(CALLBACKTYPE type, void *data);
	void ProcessWin(CALLBACKTYPE type, void *data);
	
	/* ��Ϸ״̬�Ĵ����� */
	void GameMouseMove(CPoint point);
	void GameChar(UINT nChar);
	
	/* ������״̬�Ĵ����� */
	void MenuLButtonUp(CPoint point);
	void MenuMouseMove(CPoint point);
	void MenuLButtonDown(CPoint point);
	
	/* ѡ��״̬�Ĵ����� */
	void OptionLButtonDown(CPoint point);
	void OptionMouseMove(CPoint point);
	
	/* ʤ��״̬�Ĵ����� */
	void WinLButtonDown(CPoint point);
	
	/* �������ƺ��� */
	void DrawMap(int x, int y, BOOL drawNumber = TRUE);
	void DrawCursor();
	void DrawMap();
	void DrawNumber(int x, int y);
	void DrawNumber();
	
	/* ��麯�� */
	BOOL CheckGrid(int grid);
	BOOL CheckCol(int col);
	BOOL CheckRow(int row);
	BOOL CheckFinish();
	BOOL CheckSuccess();

	/* ȡ��/�����Ѷ� */
	int GetDiffculty();
	void SetDiffculty(int dif);

protected:
	CDibSection BG;     // ��ͼ
	CDibSection View;   // ���л��ƶ��ϳ��ڱ�ͼ
	CDibSection Cursor; // ���ͼ
	CDibSection Number; // ����ͼ

	CSprite MCursor;   // �����ͼ
	CSprite MBgTop;    // ǰ����ͼ
	CSprite MBgBelow;  // ������ͼ
		
	UINT topPos;          // ǰ����ͼ�ĺ���
	UINT belowPos;        // ������ͼ�ĺ���
	CPoint CursorPos;     // ���λ�ã��������꣩
	CRect MapRect;        // ��ͼ����
	int status;           // ��Ϸ״̬
	int diffculty;        // ��Ϸ�Ѷ�
	CWnd* Window;         // ������ָ��

	struct MapData{
		int num;        // ����
		BOOL isOrigin;  // �Ƿ�����Ŀ���������
	};

	MapData Map[9][9];	   // ��¼�������е�����
};

#endif // !defined(AFX_MAPMANAGER_H__330F4346_02C5_4EF1_A397_176590884C01__INCLUDED_)
