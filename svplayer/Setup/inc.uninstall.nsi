
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SVPlayer"
  DeleteRegKey HKLM "SOFTWARE\SVPlayer"
  DeleteRegKey HKCU "SOFTWARE\SVPlayer"


  ; Remove files and uninstaller
  Delete /REBOOTOK $INSTDIR\svp*.*
  Delete /REBOOTOK $INSTDIR\mplayerc.*
  Delete /REBOOTOK $INSTDIR\Updater.exe
  Delete /REBOOTOK $INSTDIR\unrar.dll
  RMDir  /r /REBOOTOK $INSTDIR\codecs
  RMDir  $INSTDIR

  ; Remove shortcuts, if any
  Delete /REBOOTOK "$SMPROGRAMS\����Ӱ��������\*.*"
  RMDir /REBOOTOK "$SMPROGRAMS\����Ӱ��������"
  Delete /REBOOTOK "$QUICKLAUNCH\����Ӱ��.lnk"
  Delete /REBOOTOK "$DESKTOP\����Ӱ��.lnk"

  ; Remove directories used

  RMDir "$INSTDIR"