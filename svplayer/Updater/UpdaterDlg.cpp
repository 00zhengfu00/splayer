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


CUpdaterDlg::CUpdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdaterDlg::IDD, pParent)
,bHide(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
END_MESSAGE_MAP()


// CUpdaterDlg message handlers

BOOL CUpdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if(cup.downloadList()){
		AfxBeginThread(ThreadCheckUpdate , (LPVOID)&cup);
	}
	
	SetTimer(IDT_REFRESH_STAT, 700, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUpdaterDlg::OnPaint()
{
	if (IsIconic())
	{
		if(!bHide){
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
		}
	}
	else
	{
		if(bHide){
			ShowWindow(SW_HIDE);
			ShowWindow(SW_MINIMIZE);
		}else{
			CDialog::OnPaint();
		}
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
		case IDT_REFRESH_STAT:
			{
				CString szTmp;
				tnid.cbSize = sizeof(NOTIFYICONDATA); 
				tnid.hWnd = this->m_hWnd; 
				tnid.uID = IDR_MAINFRAME; 
				tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
				tnid.uCallbackMessage = WM_NOTIFYICON; 
				tnid.hIcon = this->m_hIcon; 

				szTmp.Format( _T("����Ӱ���Զ����³���\n�ļ���%d/%d ���أ�%0.2f%%") , cup.iSVPCU_CURRETN_FILE , iSVPCU_TOTAL_FILE , (double)iSVPCU_TOTAL_FILEBYTE_DONE/cup.iSVPCU_TOTAL_FILEBYTE);

				//SetWindowText(szTmp);
				wcscpy_s(tnid.szTip, szTmp);

				Shell_NotifyIcon(NIM_ADD, &tnid); 
				Shell_NotifyIcon(NIM_MODIFY,&tnid);

				if(cup.bSVPCU_DONE){
					KillTimer(IDT_REFRESH_STAT);
					SetTimer(IDT_CLOSE_DLG, 5000, NULL);
				}
			}
			break;
		case IDT_CLOSE_DLG:
			Shell_NotifyIcon(NIM_DELETE, &tnid); 
			OnOK();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CUpdaterDlg::OnBnClickedOk()
{
	
	//OnOK();
	ShowWindow(SW_MINIMIZE);
}

void CUpdaterDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_MINIMIZE);
	//CDialog::OnClose();
}

void CUpdaterDlg::OnBnClickedButton1()
{
	Shell_NotifyIcon(NIM_DELETE, &tnid); 
	OnOK();
}
