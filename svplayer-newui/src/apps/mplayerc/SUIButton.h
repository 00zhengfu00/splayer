
#pragma once

#include "stdafx.h"

#include <atlbase.h>


class CMemoryDC : public CDC
{
public:
	// Data members
	CDC* m_hDCOriginal;
	RECT m_rcPaint;
	CBitmap m_bmp;
	HBITMAP m_hBmpOld;

	// Constructor/destructor
	CMemoryDC(CDC* hDC, RECT& rcPaint) : m_hDCOriginal(hDC), m_hBmpOld(NULL)
	{
		m_rcPaint = rcPaint;
		CreateCompatibleDC(m_hDCOriginal);
		ATLASSERT(m_hDC != NULL);
		m_bmp.CreateCompatibleBitmap( m_hDCOriginal, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top);
		ATLASSERT( (HBITMAP)m_bmp != NULL);
		m_hBmpOld = (HBITMAP)SelectObject(m_bmp);
		SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
	}

	~CMemoryDC()
	{
		m_hDCOriginal->BitBlt( m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, CDC::FromHandle(m_hDC), m_rcPaint.left, m_rcPaint.top, SRCCOPY);
		SelectObject(m_hBmpOld);
	}
};

#define ALIGN_TOPLEFT 1
#define ALIGN_TOPRIGHT 2
#define ALIGN_BOTTOMLEFT 3
#define ALIGN_BOTTOMRIGHT 4
class CSUIButton {

public:
	CSize m_btnSize; //��ť�Ĵ�С�ߴ�
	int m_iAlign; //�봰�ڶ��뷽��
	CRect m_marginTownd; //��ť����ڴ��ڵ�λ�ã� 0Ϊ��ǿ�� ����Ϊ�ٷֱ�
	CRect m_rcHitest; //��ť����Ч��Χ������hittest
	CBitmap m_bitmap; //ͼƬ�洢λ��
	int m_stat; //0 normal ; 1 hove ; 2 clicked ; 3 disabled; 4 hide
	UINT m_htMsgID;
	BOOL m_hide;
	CSUIButton * m_relativeto;
	CRect m_marginToBtn;//�������һ����ť��λ��
	CString m_szBmpName;

	CSUIButton(LPCTSTR szBmpName , int iAlign, CRect marginTownd , CSUIButton * relativeto = NULL
		, BOOL bNotButton = false, UINT htMsgID = NULL , BOOL bHide = FALSE);

	void LoadImage(LPCTSTR szBmpName);

	void OnSize(CRect WndRect);

	void OnPaint(CMemoryDC *hDC);

	BOOL OnHitTest(CPoint pt , BOOL bLBtnDown);

	BOOL m_NotButton;

private:
	void PreMultiplyBitmap( CBitmap& bmp );
	LONG CalcRealMargin(LONG Mlen, LONG bW, LONG wW);
	
};



class CSUIBtnList : public CList<CSUIButton*>
{

public:
	CSUIBtnList();
	virtual ~CSUIBtnList();

	void PaintAll(CMemoryDC *hDC);
	
	void OnSize(CRect WndRect);

	UINT OnHitTest(CPoint pt );

	void SetHideStat(POSITION pos, BOOL bHide); //By Position
	void SetHideStat(UINT iMsgID, BOOL bHide); //By HT MSG ID
	void SetHideStat(LPCTSTR szBmpName, BOOL bHide); //By BitMapName
};

