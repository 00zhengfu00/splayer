#pragma once
#include "stdafx.h"
#include "mplayerc.h"



//�����ʾ���
//mouseoverʱ��ʾ��͸�������������رհ�ť�ͱ�ɫ��ť��out�����ʾ����
//Ĭ������Ļ�ײ�������ʾ������ͨ����ק�ı�λ��
//�رպ��Ϊ�������ڽ�������ʾ

class SVPLycShowBox :
	public CWnd
{
private:

	//����Ӧ��Ϊ͸��
	//ChangeBackgroundColor( COLORREF rgb_color );
	int ChangeFontColor( COLORREF rgb_color );


	/*
	��ק���ı䱳����ɫ , �������ɫ�ʱ仯
	ÿ60��һ������
	����+R ��+G ����+B
	����+R ��-G ����-B
	like a invisible color picker
	*/
	int ChangeFontColorByMouse(CPoint movement);

public:
	SVPLycShowBox(void);
	~SVPLycShowBox(void);

	/* 
	��ʾ������ֺ������ʱ��
	*/

	int ShowLycLine(CString szLycLine, int iLastingTime);

};
