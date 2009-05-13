
#pragma once

#include "stdafx.h"

#include <atlbase.h>

class CSUIButton {
	CSize m_btnSize; //��ť�Ĵ�С�ߴ�
	int m_iAlign; //�봰�ڶ��뷽��
	CRect m_marginTownd; //��ť����ڴ��ڵ�λ�ã� 0Ϊ��ǿ�� ����Ϊ�ٷֱ�
	CRect m_rcHitest; //��ť����Ч��Χ������hittest
	CBitmap m_bitmap; //ͼƬ�洢λ��
	int m_stat; //0 normal ; 1 hove ; 2 clicked ; 3 disabled; 4 hide

	
	CSUIButton * m_relativeto;
	CRect m_marginToBtn;//�������һ����ť��λ��

	CSUIButton(LPCTSTR szBmpName , int iAlign, CRect marginTownd , CSUIButton * relativeto = NULL);

	void LoadImage(LPCTSTR szBmpName);

	void PreMultiplyBitmap( CBitmap& bmp );

	void OnSize(CRect WndRect);
};