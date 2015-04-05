;NSIS Script For Cuckoo

;Version Of Your Application
!define MUI_PRODUCT "Cuckoo"
!define MUI_VERSION "4.34"

;Do A CRC Check
CRCCheck On

;Compression format
SetCompressor /SOLID lzma

;New look
XPStyle On

;Output File Name
OutFile "cuckoo-${MUI_VERSION}-install.exe"

;The Default Installation Directory
InstallDir "$PROGRAMFILES\${MUI_PRODUCT}"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${MUI_PRODUCT}" ""

;Setup icon
Icon "../res/Cuckoo.ico"
UninstallIcon "../res/Cuckoo.ico"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; First is default
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\French.nlf"

Name ${MUI_PRODUCT}

LangString SecShortcuts ${LANG_ENGLISH} "Start Menu Shortcuts"
LangString SecShortcuts ${LANG_FRENCH} "Raccourcis du Menu Démarrer"

;--------------------------------

Function .onInit
	;Language selection dialog
	Push ""
	Push ${LANG_ENGLISH}
	Push English
	Push ${LANG_FRENCH}
	Push French
	Push A		; A means auto count languages
					; for the auto count to work the first empty push (Push "") must remain
	LangDLL::LangDialog "Installer Language" "Please select the language of the installer"

	Pop $LANGUAGE
	StrCmp $LANGUAGE "cancel" 0 +2
		Abort
FunctionEnd

Function .onInstSuccess
	ExecShell "Open" "rundll32.exe" 'desk.cpl,InstallScreenSaver "$SYSDIR\${MUI_PRODUCT}.scr"' SW_SHOWNORMAL
FunctionEnd

Section "${MUI_PRODUCT}"
	SetAutoClose True
	SectionIn RO

	SetOutPath $SYSDIR
	File "${MUI_PRODUCT}.scr"
	SetOutPath $INSTDIR
	File "History.txt"
	File "Historique.txt"

	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninst.exe"
	WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NSIS" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayIcon" "$INSTDIR\Uninst.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayVersion" "${MUI_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "Publisher" "Liberty's"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "URLInfoAbout" "http://www.libertys.com/"	;Publisher's link
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "HelpLink" "http://www.beroux.com/?id=5"		;Support Information
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "URLUpdateInfo" "http://www.beroux.com/?id=5"	;Product Updates
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "NoModify" 0x00000001
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "NoRepair" 0x00000001
	WriteUninstaller "Uninst.exe"
SectionEnd

Section $(SecShortcuts)
	SetOutPath $INSTDIR
	File "normal-uninstall.ico"

	CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
	StrCmp $LANGUAGE ${LANG_ENGLISH} 0 En
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninst.exe" "" "$INSTDIR\normal-uninstall.ico" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Start the screen saver.lnk" "$SYSDIR\${MUI_PRODUCT}.scr" "-s" "$SYSDIR\${MUI_PRODUCT}.scr" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Config.lnk" "rundll32.exe" 'desk.cpl,InstallScreenSaver "$SYSDIR\${MUI_PRODUCT}.scr"' "$SYSDIR\${MUI_PRODUCT}.scr" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\History.lnk" "$INSTDIR\History.txt" "" "$INSTDIR\History.txt" 0
	En:
	StrCmp $LANGUAGE ${LANG_FRENCH} 0 Fr
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Désinstaller.lnk" "$INSTDIR\Uninst.exe" "" "$INSTDIR\normal-uninstall.ico" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Démarrer l'écran de veille.lnk" "$SYSDIR\${MUI_PRODUCT}.scr" "-s" "$SYSDIR\${MUI_PRODUCT}.scr" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Configurer.lnk" "rundll32.exe" 'desk.cpl,InstallScreenSaver "$SYSDIR\${MUI_PRODUCT}.scr"' "$SYSDIR\${MUI_PRODUCT}.scr" 0
		CreateShortCut "$SMPROGRAMS\${MUI_PRODUCT}\Historique.lnk" "$INSTDIR\Historique.txt" "" "$INSTDIR\Historique.txt" 0
	Fr:
SectionEnd

Section Uninstall
	;Delete Files
	Delete "$SYSDIR\${MUI_PRODUCT}.scr"

	;Delete Start Menu Shortcuts
	Delete "$SMPROGRAMS\${MUI_PRODUCT}\*.*"
	RMDir "$SMPROGRAMS\${MUI_PRODUCT}"

	;Delete Uninstaller And Unistall Registry Entries
	Delete "$INSTDIR\History.txt"
	Delete "$INSTDIR\Historique.txt"
	Delete "$INSTDIR\normal-uninstall.ico"
	Delete "$INSTDIR\Uninst.exe"
	DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"
	DeleteRegKey HKLM "SOFTWARE\${MUI_PRODUCT}"
	RMDir "$INSTDIR"
SectionEnd
