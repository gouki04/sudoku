//
//	DIB Section
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef	__DibSection_h__
#define	__DibSection_h__

//
//	CDIBSection class
//
class CDibSection 
{
  public:
	/* ������������ */
	CDibSection();
	CDibSection(const CDibSection &dib);
	~CDibSection();

	/* ��ʼ�������ٺ��� */
	BOOL Create(int width, int height, int depth);
	void Destroy();

	/* ����bmp������bmp */
	BOOL LoadBmp(const char *path);
	BOOL SaveBmp(const char *path);

	/* ���Ƽ��ϳɺ��� */
	void Copy(const CDibSection &dib);
	void Copy(const CDibSection &dib, CPoint to, CSize size, CPoint from);
	void Mix(const CDibSection &dib, CPoint to, CSize size, CPoint from,
		COLORREF transparent = RGB(0, 255, 0));

	/* ���ƺ��� */
	void Draw(HDC dc, int x, int y, int w, int h, int ox=0, int oy=0);
	void Draw(HDC dc, const CRect &rect, CPoint point);
	void Draw(HDC dc, const CRect &rect);
	void FillRect(const CRect &rect, COLORREF color);

	/* ȡ���غ��� */
	const void *GetBits() const { return Bits; }
	void *GetBits() { return Bits; }
	const void *GetBits(int x, int y) const;
	void *GetBits(int x, int y);

	/* ȡ���ݳ�Ա���� */
	BOOL    IsOK()          const { return Bits != 0; }
	int     Width()         const { return Header.Info.biWidth; }
	int     Height()        const { return Header.Info.biHeight; }
	int     Depth()         const { return Header.Info.biBitCount; }
	int     BytesPerLine()  const { return bytes_per_line; }
	int     BytesPerPixel() const { return bytes_per_pixel; }
	HBITMAP Handle()        const { return hBitmap; }

	/* ����=�Ų����� */
	CDibSection &operator=(const CDibSection &dib);

	/* ȡͼƬ�������ݺ��� */
	static int ScanBytes(int width, int depth);
	static int PixelBytes(int depth);

  protected:

	struct	{
		BITMAPINFOHEADER	Info;
		DWORD				BitField[3];
	} Header;

	HBITMAP	hBitmap;           //bmp�ļ����
	void    *Bits;             //bmp������ָ��
	int     bytes_per_line;    //һ�е��ֽ���
	int     bytes_per_pixel;   //һ���ص��ֽ���
} ;

/************************************************************************/
/* CDibDC ��                                                            */
class CDibDC: public CDC {
  public:
	/* ����/�������� */
	CDibDC(const CDibSection &dib);
	CDibDC(const CDibSection &dib, HDC hDC);
	~CDibDC();

  protected:
	HDC     hScreenDC;    //�豸DC���
	HGDIOBJ hOldBitmap;   //��bmp�ľ��
} ;
/************************************************************************/


// ************************************************************************
//  �������������£�
//
//  CDibSection�ࣺ
//
//  PixelBytes()        - �õ�һ������ռ���ֽ���
//  ScanBytes()         - �õ�һ����ռ���ֽ���
//  
//  GetBits()           - �õ�ָ�����ص��ָ��ĺ���
//  GetBits()           - 
//
//  Draw()              - ���ص�3�����ƺ���
//  Draw()		        - 
//  Draw()		        - 
//
// ************************************************************************


/*************************************************************************
 *
 * �������ƣ�
 *   CDibDC()
 *
 * ����:
 *   const CDibSection &dib     - �����Ƶ�CDibSection����
 *
 * ����ֵ:
 *   void
 *
 * ˵��:
 *   �������캯������һ��CDibSection�����ʼ��һ��CDibDC����
 *
 ************************************************************************/

inline CDibDC::CDibDC(const CDibSection &dib)
{
	hScreenDC = ::GetDC(0);
	HDC	memDC = ::CreateCompatibleDC(hScreenDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}


/*************************************************************************
 *
 * �������ƣ�
 *   CDibDC()
 *
 * ����:
 *   const CDibSection &dib     - �����Ƶ�CDibSection����
 *   HDC hDC                    - ����豸DC���
 *
 * ����ֵ:
 *   void
 *
 * ˵��:
 *   �������캯��
 *
 ************************************************************************/

inline CDibDC::CDibDC(const CDibSection &dib, HDC hDC)
{
	hScreenDC = 0;
	HDC	memDC = ::CreateCompatibleDC(hDC);
	hOldBitmap = ::SelectObject(memDC, dib.Handle());
	Attach(memDC);
}

/*************************************************************************
 *
 * �������ƣ�
 *   ~CDibDC()
 *
 * ����:
 *
 * ����ֵ:
 *   void
 *
 * ˵��:
 *   ��������
 *
 ************************************************************************/

inline CDibDC::~CDibDC()
{
	::GdiFlush();
	HDC memDC = Detach();
	::SelectObject(memDC, hOldBitmap);
	::DeleteDC(memDC);
	if (hScreenDC)
		::ReleaseDC(0, hScreenDC);
}


/*************************************************************************
 *
 * �������ƣ�
 *   PixelBytes()
 *
 * ����:
 *   int depth             - ͼƬ��ȣ�biBitCountÿ������ռλ����
 *
 * ����ֵ:
 *   int                   - ����һ������ռ���ֽ���
 *
 * ˵��:
 *   ��Ϊ1�ֽڰ���8λ����С��1�ֽڵ�1�ֽ��㣬���Խ���depth+7��/8���ý��
 *
 ************************************************************************/

inline int CDibSection::PixelBytes(int depth)
{
	return (depth + 7) / 8;
}


/*************************************************************************
 *
 * �������ƣ�
 *   ScanBytes()
 *
 * ����:
 *   int width             - ͼƬ�Ŀ�ȣ�������Ϊ��λ��
 *   int depth             - ͼƬ��ȣ�biBitCountÿ������ռλ����
 *
 * ����ֵ:
 *   int                   - ����һ����ռ���ֽ���
 *
 * ˵��:
 *   window�涨һ��ɨ������ռ���ֽ�������Ϊ4����������������4�ı�����Ҫ��
 *   �������䡣�������¹�ʽ�������һ��ʵ����ռ���ֽ���
 *
 ************************************************************************/

inline int CDibSection::ScanBytes(int width, int depth)
{
	return ((width * PixelBytes(depth) + 3) / 4) * 4;
}


/*************************************************************************
 *
 * �������ƣ�
 *   GetBits() const
 *
 * ����:
 *   int x                 - ���ص��x����
 *   int y                 - ���ص��y����
 *
 * ����ֵ:
 *   const void*           - ָ������ص��ָ��
 *
 * ˵��:
 *   ��ΪBMP�ļ�������ԭ�������½ǣ���ϰ���ϵ����������Ͻǣ�����ȡ����ʱ����
 *   ���������һ�£�����Ϊheight - y
 *
 ************************************************************************/

inline const void *CDibSection::GetBits(int x, int y) const
{
	return (const void *)((const char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}


/*************************************************************************
 *
 * �������ƣ�
 *   GetBits()
 *
 * ����:
 *   int x                 - ���ص��x����
 *   int y                 - ���ص��y����
 *
 * ����ֵ:
 *   void*           - ָ������ص��ָ��
 *
 * ˵��:
 *   ��ΪBMP�ļ�������ԭ�������½ǣ���ϰ���ϵ����������Ͻǣ�����ȡ����ʱ����
 *   ���������һ�£�����Ϊheight - y
 *
 ************************************************************************/

inline void *CDibSection::GetBits(int x, int y)
{
	return (void *)((char *)GetBits()
		+ (Height() - y - 1) * bytes_per_line + x * bytes_per_pixel);
}


/*************************************************************************
 *
 * �������ƣ�
 *   Draw()
 *
 * ����:
 *   HDC dc                - ����豸DC�ľ��
 *   int x                 - ���λ�õ����Ͻǵ�x���꣨��Ӧ��Ŀ��DC��
 *   int y                 - ���λ�õ����Ͻǵ�y���꣨��Ӧ��Ŀ��DC��
 *   int w                 - ���Ƶ������С�Ŀ��
 *   int h                 - ���Ƶ������С�ĸ߶�
 *   int ox                - ���λ�õ����Ͻǵ�x���꣨��Ӧ��ԴDC��
 *   int oy                - ���λ�õ����Ͻǵ�y���꣨��Ӧ��ԴDC��
 *
 * ����ֵ:
 *   void         
 *
 * ˵��:
 *   ֱ��ʹ��CDC::BitBlt������
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, int x, int y, int w, int h, int ox, int oy)
{
	BitBlt(dc, x, y, w, h, CDibDC(*this, dc), ox, oy, SRCCOPY);
}


/*************************************************************************
 *
 * �������ƣ�
 *   Draw()
 *
 * ����:
 *   HDC dc                - ����豸DC�ľ��
 *   const CRect& rect     - ����ľ������������Ӧ��Ŀ��DC��
 *   CPoint point          - ���λ�õ����Ͻ����꣨��Ӧ��Ŀ��DC��
 *
 * ����ֵ:
 *   void         
 *
 * ˵��:
 *   ֱ��ʹ��CDC::BitBlt������
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, const CRect &rect, CPoint point)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), point.x, point.y);
}


/*************************************************************************
 *
 * �������ƣ�
 *   Draw()
 *
 * ����:
 *   HDC dc                - ����豸DC�ľ��
 *   const CRect& rect     - ����ľ�������
 *
 * ����ֵ:
 *   void         
 *
 * ˵��:
 *   �����е�rect�����Ͻ�����ֵ��Ӧ��ԴDC�Լ�Ŀ��DC��Ҳ����˵��ֻ�е�
 *   ���λ�õ����Ͻ���ԴDC�Լ�Ŀ��DC������ͬʱ�ſ����ô˺���
 *
 ************************************************************************/

inline void CDibSection::Draw(HDC dc, const CRect &rect)
{
	Draw(dc, rect.left, rect.top, rect.Width(), rect.Height(), rect.left, rect.top);
}

#endif
