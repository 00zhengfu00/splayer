!include "inc.Var.nsi"

Function .onInit
 !include "inc.OnInt.nsi"

FunctionEnd

OutFile "..\..\NetSetup.exe"

; Uninstaller

Section "Uninstall"
  
  !include "inc.uninstall.nsi"
  
SectionEnd




!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${mplayerc} "�����������򣬱��밲װ"

!insertmacro MUI_FUNCTION_DESCRIPTION_END
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS