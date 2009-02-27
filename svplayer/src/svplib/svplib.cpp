#include "svplib.h"
#include "SVPNet.h"
#include "SVPHash.h"
#include <shlobj.h>

class CSVPSubUploadThreadData{
public:
	CString fnVideoFilePath;
	CString szSubPath;
	int iDelayMS;
};
static CAtlList<CString> * szGStatMsg = NULL;
void SVP_RealUploadSubFileByVideoAndSubFilePath(CString fnVideoFilePath, CString szSubPath, int iDelayMS){

	CSVPNet svpNet;
	CSVPhash svpHash;
	CSVPToolBox svpToolBox;
	CString szFileHash  = svpHash.ComputerFileHash(fnVideoFilePath);

	CStringArray szaSubFiles;
	svpToolBox.FindAllSubfile(szSubPath, &szaSubFiles);

	CString szSubHash = svpHash.ComputerSubFilesFileHash(&szaSubFiles);
	SVP_LogMsg(CString("Got Sub Hash ") + svpToolBox.Implode( _T(" | "), &szaSubFiles) + _T(" -> ") + szSubHash );

	if ( svpNet.WetherNeedUploadSub(fnVideoFilePath,szFileHash, szSubHash,iDelayMS) ){
		svpNet.UploadSubFileByVideoAndHash(fnVideoFilePath,szFileHash, szSubHash, &szaSubFiles,iDelayMS);
		return ;
	}
}
UINT __cdecl SVPThreadUploadSubFile( LPVOID lpParam ) 
{ 

	CSVPSubUploadThreadData* pData = (CSVPSubUploadThreadData*)lpParam;
	SVP_RealUploadSubFileByVideoAndSubFilePath(pData->fnVideoFilePath,  pData->szSubPath, pData->iDelayMS);

	delete pData;
	return 0; 
} 
UINT __cdecl SVPThreadCheckUpdaterExe( LPVOID lpParam ) 
{ 
	if(lpParam){
		szGStatMsg = (CAtlList<CString> *) lpParam;
	}
	SVP_RealCheckUpdaterExe();
	szGStatMsg = NULL;
	return 0; 
}
void SVP_RealCheckUpdaterExe(){
	//��� updater.exe �Ƿ��д
	CSVPToolBox svpToolBox;
	CSVPNet svpNet;
	CString szUpdaterPath = svpToolBox.GetPlayerPath(_T("Updater.exe"));
	if ( svpToolBox.isWriteAble(szUpdaterPath) ){
		//�����д�����ļ��汾��+�ļ�����hash ���͸� http://svplayer.shooter.cn/api/updater.php 

		CString FileVersionHash = svpToolBox.getFileVersionHash(szUpdaterPath);

		// �������ж��Ƿ���Ҫ���� updater.exe �� �������Ҫ����������404�������Ҫ�������򷵻�updater.exe������
		if ( svpNet.CheckUpdaterExe(FileVersionHash, szUpdaterPath) ){

		}
		SVP_LogMsg( _T("��⵽�µİ汾����������������") ); 
		//������������
		ShellExecute( NULL, _T("open"), szUpdaterPath, _T("") , _T(""), SW_SHOW);	
	}
}
void SVP_CheckUpdaterExe(){
	
	AfxBeginThread( SVPThreadCheckUpdaterExe, NULL);
}

void SVP_FetchSubFileByVideoFilePath(CString fnVideoFilePath, CStringArray* szSubArray, CAtlList<CString> * szStatMsg){
	
	szGStatMsg = szStatMsg;
	SVP_LogMsg(_T("����ͨ������Ӱ����Ļ����ƥ��ϵͳѰ����Ļ"), 31);
	CSVPNet svpNet;
	CSVPhash svpHash;
	CString szFileHash  = svpHash.ComputerFileHash(fnVideoFilePath);
	if ( svpNet.QuerySubByVideoPathOrHash(fnVideoFilePath,szFileHash) ){
		return ;
	}

		//load sub file to sublist
		CString szSubFilePath;
		int iSubTotal = 0;
		for(int i = 0; i < svpNet.svpToolBox.szaSubTmpFileList.GetCount(); i++){
			szSubFilePath = svpNet.svpToolBox.getSubFileByTempid(i, fnVideoFilePath);
			if(szSubFilePath){
				szSubArray->Add(szSubFilePath);
				SVP_LogMsg(CString(_T("�ɹ����ص���Ļ�ļ� ")) + szSubFilePath , 31 ); //TODO: if its vobsub, load perfered language
				iSubTotal++;
			}else{
				SVP_LogMsg(_T("Fail to get sub file name"));
			}
		}

	if(iSubTotal) {
		CString szLog;
		szLog.Format(_T("ͨ������Ӱ����Ļ����ƥ��ϵͳ�ɹ����ص� %d ����Ļ�ļ� "), iSubTotal);
		SVP_LogMsg( szLog, 31 ); 
	}

	szGStatMsg = NULL;
	return;
}
void SVP_UploadSubFileByVideoAndSubFilePath(CString fnVideoFilePath, CString szSubPath, int iDelayMS){
	CSVPSubUploadThreadData* pData = new CSVPSubUploadThreadData();
	pData->fnVideoFilePath = fnVideoFilePath;
	pData->szSubPath = szSubPath;
	pData->iDelayMS = iDelayMS;
	AfxBeginThread(SVPThreadUploadSubFile, pData); 
}

void SVP_LogMsg(CString logmsg, int level){

	if(szGStatMsg && (level & 16) ){
		szGStatMsg->AddTail(logmsg);
	}
	CStdioFile f;
	CSVPToolBox svpToolBox;
	CString szLogPath = svpToolBox.GetPlayerPath(_T("SVPDebug.log"));
	if(f.Open(szLogPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::typeBinary))
	{
		f.SeekToEnd();
		f.WriteString(logmsg+_T("\r\n"));
		
		f.Flush();
		f.Close();
	}
		
}

BOOL SVP_SetCoreAvcCUDA(BOOL useCUDA){
	HRESULT hr;
	LPITEMIDLIST pidl;
	hr = SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA, &pidl);
	if (hr)
		return false;
	

	TCHAR szPath[MAX_PATH];
	BOOL f = SHGetPathFromIDList(pidl, szPath);
	PathAddBackslash(szPath); 
	PathAppend(szPath, _T("coreavc.ini"));

	CSVPToolBox svpTool;
	
		//TODO: check VGA card and driver version
		//$mooi(vcardmake) $mooi(vcardproc)   $mooi(vcarddriver)
		//if ($1 == vcardmake) { return $wmiget(Win32_VideoController).AdapterCompatibility }
		//if ($1 == vcardproc) { return $wmiget(Win32_VideoController).VideoProcessor }
		//if ($1 == vcarddriver) { return $wmiget(Win32_VideoController).DriverVersion }

		if(useCUDA){
			HMODULE hDll = ::LoadLibrary(_T("nvcuda.dll"));
			
			if (hDll)
			{
				CString dllpath;
				GetModuleFileName(hDll, dllpath.GetBuffer(MAX_PATH), MAX_PATH);
				dllpath.ReleaseBuffer();

				//get build
				svpTool.getFileVersionHash(dllpath);
				if ( svpTool.dwBuild ){
					if(svpTool.dwBuild > 8205 || svpTool.dwBuild < 1105){ // not sure when will build version over 200.xx
						//ok
						useCUDA = true;
					}else{
						useCUDA = false;
					}
				}

				//get version
				/*
					NvAPI_Status nvapiStatus;
					NV_DISPLAY_DRIVER_VERSION version = {0};
					version.version = NV_DISPLAY_DRIVER_VERSION_VER;
					nvapiStatus = NvAPI_Initialize();
					nvapiStatus = NvAPI_GetDisplayDriverVersion (NVAPI_DEFAULT_HANDLE, &version);
					if(nvapiStatus != NVAPI_OK) {... inform user nvidia card not found ...}
					if(version.drvVersion < DESIRED_NVIDIA_DRIVER_VERSION) {... inform user nvidia driver version not the one you wanted ...}
				*/

				
				
			}else{
				useCUDA = false;
			}
		}

		FILE*   fileHandle = _wfopen(  szPath , _T("r+") );
		if(fileHandle){
			char szStr[8093];
			int iRead = fread_s(szStr, 8093, sizeof( char ), 8093, fileHandle);
			CStringA szBuf(szStr , iRead) ;

			if(szBuf.IsEmpty()){
				szBuf =  "[CoreAVC]\r\n";
				if(useCUDA){
					szBuf += "use_cuda=1";
				}else{
					szBuf += "use_cuda=-1";
				}
			}else{
				if(useCUDA){
					if (!szBuf.Replace(CStringA("use_cuda=-1") , CStringA("use_cuda=1")) && szBuf.Find(CStringA("use_cuda=1")) < 0 ){
						szBuf += CStringA(" use_cuda=1");
					}
				}else{
					szBuf.Replace(CStringA("use_cuda=1") , CStringA("use_cuda=-1"));
				}
			}

			szBuf.Trim();
			fseek( fileHandle , SEEK_SET , SEEK_SET);
			fwrite(szBuf,sizeof( char ),szBuf.GetLength(), fileHandle ) ;
			SetEndOfFile(fileHandle);
			fclose(fileHandle);
		
		}

		
	
	return useCUDA;
}

