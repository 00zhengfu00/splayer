// UpdaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Updater.h"
#include "UpdaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpdaterDlg dialog

UINT __cdecl ThreadCheckUpdate( LPVOID lpParam ) 
{ 
	cupdatenetlib* cup = (cupdatenetlib*)lpParam;
	
	cup->procUpdate();

	
	return 0; 
}
CString getSizeMeasure(__int64 iSize){
	CString measure = _T("B");
	if(iSize > 1024) iSize /= 1024, measure = L"KB";
	if(iSize > 1024) iSize /= 1024, measure = L"MB";
	if(iSize > 1024) iSize /= 1024, measure = L"GB";
	return measure;
}
CString getShortSize(__int64 iSize){
	
	double shortsize = iSize;
	CString measure = _T("B");
	if(shortsize > 1024) shortsize /= 1024, measure = L"KB";
	if(shortsize > 1024) shortsize /= 1024, measure = L"MB";
	if(shortsize > 1024) shortsize /= 1024, measure = L"GB";

	CString szRet ;
	szRet.Format(_T("%0.2f%s"), shortsize , measure);
	return  szRet;
}
CUpdaterDlg::CUpdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdaterDlg::IDD, pParent)
,bHide(1)
,m_bGoodToGo(0)
,verbose(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, STATIC_CURRENT, csCurTask);
	DDX_Control(pDX, STATIC_TOTAL, csTotalProgress);
	DDX_Control(pDX, IDC_PROGRESS1, prg_total);
	DDX_Control(pDX, IDC_STATIC_DONE, cs_stat);
	DDX_Control(pDX, IDC_SYSLINK1, cslink);
	DDX_Control(pDX, IDC_BUTTON1, cb_stop);
	DDX_Control(pDX, IDC_STATIC_SPEED, szSpeed);
	DDX_Control(pDX, STATIC_CURRENT2, cszSizeTotal);
}

BEGIN_MESSAGE_MAP(CUpdaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_NOTIFYICON, On_WM_NOTIFYICON)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CUpdaterDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CUpdaterDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CUpdaterDlg::OnNMClickSyslink1)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CUpdaterDlg message handlers

BOOL CUpdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	prg_total.SetRange(0, 1000);
	
	HDC hdc = ::GetDC(NULL);
	double dDefaultSize = 22;
	double dIntroSize = 14;
	double scale = 1.0;//*GetDeviceCaps(hdc, LOGPIXELSY) / 96.0;
	::ReleaseDC(0, hdc);

	m_hBigFont.m_hObject = NULL;
	

	if(!(::GetVersion()&0x80000000)){
		m_hBigFont.CreateFont(int(dDefaultSize * scale), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, 
		_T("Microsoft Sans Serif"));

		m_hIntroFont.CreateFont(int(dIntroSize * scale), 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, 
			_T("Microsoft Sans Serif"));
	}
	if(!m_hBigFont.m_hObject){
		m_hBigFont.CreateFont(int(dDefaultSize * scale), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, 
		_T("MS Sans Serif"));


		m_hIntroFont.CreateFont(int(dIntroSize * scale), 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, 
			_T("MS Sans Serif"));

	}


	cs_stat.SetWindowText(_T("�������������°�����ֲ����������ڸ�����ɺ�5�������˳�"));
	csCurTask.SetWindowText(_T("��ǰ�������ڼ���������"));
	SetTimer(IDT_START_CHECK, 1000, NULL);

	
	tnid.cbSize = sizeof(NOTIFYICONDATA); 
	tnid.hWnd = this->m_hWnd; 
	tnid.uID = IDR_MAINFRAME; 
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	tnid.uCallbackMessage = WM_NOTIFYICON; 
	tnid.hIcon = this->m_hIcon; 
	wcscpy_s(tnid.szTip, _T("����Ӱ���������Զ����³���"));
	Shell_NotifyIcon(NIM_ADD, &tnid); 

	szaIntro.Add(_T("ֻҪ�����������á�������ק���������ס����Ϳ��Է���ĸı仭�����"));
	szaIntro.Add(_T("�ڽ��������п��������Զ���ı�������"));
	szaIntro.Add(_T("���ֲ��������谲װ������������ȫ�ܽ���"));

	szaIntro.Add(_T("��������������Ӳ��������٣�DXVA��DXVA2��EVR��CUDA"));
	szaIntro.Add(_T("ʮ���ֻ���Ч��������ϣ�ȫ�µ���Ƶ�������顣"));
	szaIntro.Add(_T("ȫ�Զ�������Ļƥ�䡣�����������Ҳ�������ռ���ֻҪһ�˲��Ź���ͬ�⹲��ȫ���˶����ڲ���ʱ�õ�ǡ��ƥ��ӰƬ����Ļ��"));
	szaIntro.Add(_T("˫��Ļ��ʾ��ͬʱ��ʾ��Ӣ˫���ͬѧϰ��ߡ�"));
	szaIntro.Add(_T("��ɫ���ⰲװ��ͬʱ֧�ֺ�����Ƶ��ʽ��������å��"));
	szaIntro.Add(_T("���ڲ�ͬ����ĻĬ����������ɫ���Կ��Ӿ�ƣ�͡�"));
	szaIntro.Add(_T("����ʶ����������Ļ��������룬Ҳ�������ֶ�ת�롣"));
	szaIntro.Add(_T("�Զ���СӢ�����룬˫����Ļ���������š�"));
	szaIntro.Add(_T("����ʹ��ϰ���Ż��Ҽ��˵�����������л���Ļ����ʾģʽ��"));
	szaIntro.Add(_T("�������á�����Ŀ�����壬��������������������ѡ�"));
	szaIntro.Add(_T("�Զ�������ʱ�������°汾���������¹��ܡ�"));
	szaIntro.Add(_T("���ֲ�����ӵ��GPL��ȨЭ�飬�ǿ�Դ�����"));

	if(bHide){
		ShowWindow(SW_HIDE);
		ShowWindow(SW_MINIMIZE);
	}
	return FALSE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUpdaterDlg::OnPaint()
{
	if (IsIconic())
	{
		
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		
	}else{

		//CDialog::OnPaint();
		
		CPaintDC dc(this);
		dc.DrawIcon(34, 25, m_hIcon);
		dc.SetBkMode(TRANSPARENT);
		HFONT oldFont = (HFONT)dc.SelectObject((HFONT) m_hBigFont);
		dc.SetTextColor( 0x353535);
		dc.DrawText(_T("���ֲ�����\r\n�Զ���������"), CRect(80, 25, 230,85),DT_LEFT );
		dc.SelectObject((HFONT) m_hIntroFont);
		dc.DrawText(_T("������ܣ�"), CRect(39, 110, 200,130),DT_LEFT );

		dc.SelectObject((HFONT) oldFont);
	}
	
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUpdaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUpdaterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( nType == SIZE_MINIMIZED )
	{
		this->ShowWindow(SW_HIDE);
	}
}


LRESULT CUpdaterDlg::On_WM_NOTIFYICON(WPARAM wParam, LPARAM lParam) 
{ 
	UINT uID, uMouseMsg; 

	uID = (UINT) wParam; 
	uMouseMsg = (UINT) lParam; 

	if ( uID == IDR_MAINFRAME && ( uMouseMsg == WM_LBUTTONUP || uMouseMsg == WM_RBUTTONUP)){

		bHide = FALSE;
 		this->ShowWindow(SW_SHOW);
 		this->ShowWindow(SW_RESTORE);
 		this->BringWindowToTop();
	}
	return 1; 
}
void CUpdaterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent){
		case IDT_START_CHECK:
			{
				KillTimer(IDT_START_CHECK);

				
				if(cup.downloadList()){
					
					cs_stat.SetWindowText(_T("���ڸ��µ����ֲ����������°汾..."));

					csCurTask.SetWindowText(_T("��ǰ���� ���ڸ���..."));
					
					cszSizeTotal.SetWindowText(  CString(L"�ļ���С�� ") + getShortSize(cup.iSVPCU_TOTAL_FILEBYTE) );

					csTotalProgress.SetWindowText( CString(L"�����أ� ") + getShortSize(cup.iSVPCU_CURRENT_FILEBYTE_DONE) );

					SetTimer(IDT_REAL_START_CHECK, 800, NULL);


					SetTimer(IDT_SHOW_INTRO,3500, NULL);

					
				}else{
					cs_stat.SetWindowText(_T("���Ѿ�ӵ�����°�����ֲ�������Ŀǰ����������\r\n����������30����˳�"));
					csCurTask.SetWindowText(_T("��ǰ��������׼���ر�..."));
					SetTimer(IDT_CLOSE_DLG, 30000, NULL);
				}

			}
			break;
		case IDT_SHOW_INTRO:
			cs_stat.SetWindowText(szaIntro.GetAt( rand() % szaIntro.GetCount() ));
			break;
		case IDT_REAL_START_CHECK:
			{
				
				KillTimer(IDT_REAL_START_CHECK);
				cb_backgd.EnableWindow(true);
				AfxBeginThread(ThreadCheckUpdate , (LPVOID)&cup);
				csCurTask.SetWindowText(_T("��ǰ���� ��������..."));
				iTimeStart = time(NULL);
				SetTimer(IDT_REFRESH_STAT, 700, NULL);
			}
			break;
		case IDT_REFRESH_STAT:
			{
				CString szTmp;
				

				if(cup.iSVPCU_TOTAL_FILEBYTE < cup.iSVPCU_TOTAL_FILEBYTE_DONE + cup.iSVPCU_CURRENT_FILEBYTE_DONE){
					cup.iSVPCU_TOTAL_FILEBYTE = cup.iSVPCU_TOTAL_FILEBYTE_DONE + cup.iSVPCU_CURRENT_FILEBYTE_DONE;
				}
				if (cup.iSVPCU_TOTAL_FILEBYTE  <= 0){
					cup.iSVPCU_TOTAL_FILEBYTE = 1;
				}
				double progress = 0;
				if(cup.iSVPCU_TOTAL_FILEBYTE){
					progress = (double)( cup.iSVPCU_TOTAL_FILEBYTE_DONE + cup.iSVPCU_CURRENT_FILEBYTE_DONE ) * 100/ (cup.iSVPCU_TOTAL_FILEBYTE);
				}
				//szTmp.Format( _T("����Ӱ���Զ����³���\n�ļ���%d/%d ���أ�%0.2f%%") , cup.iSVPCU_CURRETN_FILE , cup.iSVPCU_TOTAL_FILE ,progress );
				//SVP_LogMsg(szTmp);

				if(cup.bWaiting){
					szTmp = _T("�رղ������������������Զ����µ����°汾");
					csCurTask.SetWindowText(_T("��ǰ�������ڸ���..."));
					
				}
				wcscpy_s(tnid.szTip, szTmp);

//szTmp.Format( _T("���������ļ��� %s ��%d / %d��") , cup.szCurFilePath , cup.iSVPCU_CURRETN_FILE , cup.iSVPCU_TOTAL_FILE);
				if(!cup.bWaiting){
					
					double downloadedTotal = cup.iSVPCU_TOTAL_FILEBYTE_DONE + cup.iSVPCU_CURRENT_FILEBYTE_DONE;
					UINT timeNowNow = time(NULL);
					if( timeNowNow  - iTimeStart){
						double speed = downloadedTotal / ( timeNowNow  - iTimeStart);
						CString szBufSpeed;
						szBufSpeed.Format( _T("�ٶȣ� %s/��") , getShortSize(speed) );
						szSpeed.SetWindowText(szBufSpeed);
					}


					csTotalProgress.SetWindowText( CString(L"�����أ� ") + getShortSize(downloadedTotal) );

					prg_total.SetPos(int(progress * 10));
					
				}
				//SetWindowText(szTmp);

				//szTmp.Format( _T("�ܽ��ȣ�%0.2f%%") , progress);
				

				
				//Shell_NotifyIcon(NIM_MODIFY,&tnid);

				if(cup.bSVPCU_DONE){
					KillTimer(IDT_REFRESH_STAT);
					szTmp = _T("����Ӱ���������Ѿ����µ����°汾��\r\n��������2�����ڹر�");
					cs_stat.SetWindowText(szTmp);
					csCurTask.SetWindowText(_T("��ǰ���� �����ѽ���"));
					KillTimer(IDT_SHOW_INTRO);
					//cb_backgd.SetWindowText(_T("�ر�"));
					SetTimer(IDT_CLOSE_DLG, 120000, NULL);
				}
			}
			break;
		case IDT_CLOSE_DLG:
			KillTimer(IDT_CLOSE_DLG);
			Shell_NotifyIcon(NIM_DELETE, &tnid); 
			OnOK();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CUpdaterDlg::OnBnClickedOk()
{
	
	//OnOK();
	if(m_bGoodToGo){
		KillTimer(IDT_REAL_START_CHECK);
		SetTimer(IDT_REAL_START_CHECK, 1700, NULL);
	}else{
		ShowWindow(SW_MINIMIZE);
	}
}

void CUpdaterDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	Shell_NotifyIcon(NIM_DELETE, &tnid); 
	OnOK();
	//CDialog::OnClose();
}

void CUpdaterDlg::OnBnClickedButton1()
{
	Shell_NotifyIcon(NIM_DELETE, &tnid); 
	OnOK();
	/*
		if(!cup.bWaiting && cup.bSVPCU_DONE){
				Shell_NotifyIcon(NIM_DELETE, &tnid); 
				OnOK();
			}else
				ShowWindow(SW_MINIMIZE);*/
		
	
}

void CUpdaterDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	ShellExecute( NULL, _T("open"), _T("http://blog.splayer.org"), _T("") , NULL , SW_SHOW);

	*pResult = 0;
}

int CUpdaterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style &= ~WS_VISIBLE;;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
