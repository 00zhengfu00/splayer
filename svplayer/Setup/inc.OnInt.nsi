	System::Call 'kernel32::CreateMutexA(i 0, i 0, t "SVPlayerInstallerMutex")  i .r1 ?e'
	Pop $R0

	StrCmp $R0 0 texsit
	  MessageBox MB_OK|MB_ICONEXCLAMATION "��һ����װ�����Ѿ����������ˣ�"
	  Abort

   texsit: