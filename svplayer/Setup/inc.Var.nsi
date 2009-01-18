SetCompressor /SOLID lzma

 
;Name and file
Name "����Ӱ��������"
Caption "����Ӱ����������װ����"
BrandingText "����Ӱ��������"


AutoCloseWindow true
SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal
;BGGradient 000000 800000 FFFFFF
InstallColors FF8080 000030
XPStyle on

;Default installation folder
InstallDir "$PROGRAMFILES\SVPlayer"


InstallDirRegKey HKLM "Software\SVPlayer "Install_Dir"



;--------------------------------
;Interface Settings


 !include "MUI.nsh"

;!define MUI_HEADERIMAGE
;!define MUI_ABORTWARNING
;!define MUI_HEADERIMAGE_BITMAP "header.bmp"
;!define MUI_WELCOMEFINISHPAGE_BITMAP "left.bmp"
;!define MUI_UNWELCOMEFINISHPAGE_BITMAP "left.bmp"
;!define  MUI_ICON "SVPlayer.ico"
;!define  MUI_UNICON "SVPlayer.ico"
!insertmacro MUI_PAGE_LICENSE "License.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\mplayerc.exe"
!insertmacro MUI_PAGE_FINISH

;!insertmacro MUI_UNPAGE_WELCOME
;!insertmacro MUI_UNPAGE_COMPONENTS

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Pages

;!insertmacro MUI_PAGE_WELCOME


;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
;Installer Sections

;--------------------------------


; Pages
/*
Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles*/



SectionGroup /e "����Ӱ��"
; The stuff to install
Section  "����Ӱ��������" mplayerc

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  File "..\..\svplayer.bin\mplayerc.exe"
  File "..\..\svplayer.bin\Updater.exe"
  File "..\..\svplayer.bin\unrar.dll"
;  File "..\..\svplayer.bin\VSFilter.dll"
;  RegDLL $INSTDIR\VSFilter.dll
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SVPlayer" "DisplayName" "����Ӱ��������"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SVPlayer" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SVPlayer" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SVPlayer" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  WriteRegStr HKLM "SOFTWARE\SVPlayer" "Install_Dir" "$INSTDIR"
SectionEnd

Section "����Ϳ�ʼ�˵���"
  CreateDirectory "$SMPROGRAMS\����Ӱ��������"
  CreateShortCut "$SMPROGRAMS\����Ӱ��������\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\����Ӱ��������\����Ӱ��.lnk" "$INSTDIR\mplayerc.exe" "" "$INSTDIR\mplayerc.exe" 0


SectionEnd

Section "�����ݷ�ʽ"
  CreateShortCut "$DESKTOP\����Ӱ��.lnk" "$INSTDIR\mplayerc.exe"

SectionEnd

Section "�����ݲ˵�"
  CreateShortCut "$QUICKLAUNCH\����Ӱ��.lnk" "$INSTDIR\mplayerc.exe"
SectionEnd
;--------------------------------
SectionGroupEnd