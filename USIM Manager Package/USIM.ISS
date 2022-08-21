; InnoScript Version 2.3.8
; Randem Systems, Inc.
; Copyright 2003, 2004
; website:  http://www.randem.com
; support:  http://www.innoscript.com/cgi-bin/discus/discus.cgi

[Setup]
AppId=UniversalInterfaceManager
AppName=Universal Interface Manager (ALPHA)
AppVerName=GOBI Universal Interface Manager 0.2.1.2
DefaultGroupName=Beta Innovations
AppPublisher=GOBI
AppPublisherURL=
AppVersion=0.1.0.0
;AppSupportURL=http://www.yourwebsite.com
AppUpdatesURL=
AllowNoIcons=yes
;InfoAfterFile=ReadMe.txt
;WizardImageFile=yourlogo.bmp
AppCopyright=Copyright 2009-2022 GOBI
PrivilegesRequired=admin
OutputBaseFilename=usim_v0212
DefaultDirName={pf}\Beta Innovations\Universal Interface Manager
LicenseFile=.\Support\install_files\license.rtf
WizardSmallImageFile=.\Support\install_files\WizModernSmallImage.bmp
WizardImageFile=.\Support\install_files\WizModernImage.bmp
SetupIconFile=.\Support\install_files\usim.ico
UninstallIconFile=.\Support\install_files\waste.ico
UsePreviousAppDir=yes
UsePreviousGroup=yes
UpdateUninstallLogAppName=yes
VersionInfoVersion=1.0.0.0

[Tasks]
Name: desktopicon; Description: Create a &desktop icon; GroupDescription: Additional icons:
Name: quicklaunchicon; Description: Create a &Quick Launch icon; GroupDescription: Additional icons:; Flags: unchecked

[Files]
; begin VC system files
;Source: vcredist_x86.exe; DestDir: {tmp}; Flags: ignoreversion; AfterInstall: RunThis
Source: ".\Support\install_files\vcfiles\vcredist_x86.exe"; DestDir: "{tmp}"; Flags: ignoreversion;
; end VC system files

; application files
Source: .\Support\usim.exe; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
Source: .\Support\usimx64.exe; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
Source: .\Support\rfip.dll; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
Source: .\Support\xpip.xpl; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
; HTML Files
Source: .\Support\license.html; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
; Help files
;Source: .\Support\usim.pdf; DestDir: {app}; MinVersion: 4.1,4.0; Flags:  ignoreversion
; misc files

[INI]
Filename: {app}\usim.url; Section: InternetShortcut; Key: URL; String: https://github.com/LLac/usim

[Icons]
Name: {group}\Universal Interface Manager\Universal Interface Manager; Filename: {app}\usim.exe; WorkingDir: {app}
;Name: {group}\Universal Interface Manager\Universal Interface Manager Help; Filename: {app}\usim.pdf; WorkingDir: {app}
Name: {group}\Universal Interface Manager\License Agreement; Filename: {app}\license.html; WorkingDir: {app}
;Name: {group}\Universal Interface Manager\Beta Innovations on the Web; Filename: {app}\usim.url
Name: {group}\Universal Interface Manager\Uninstall Universal Interface Manager; Filename: {uninstallexe}
Name: {userdesktop}\Universal Interface Manager; Filename: {app}\usim.exe; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Universal Interface Manager; Filename: {app}\usim.exe; Tasks: quicklaunchicon; WorkingDir: {app};

[Run]
; Install only on Windows 98, Me, and NT 4.0
Filename: {app}\usim.exe; Description: Launch Universal Interface Manager; Flags: nowait postinstall skipifsilent; WorkingDir: {app}
;Exec(ExpandConstant('{app}\vcredist_x86.exe'), '/Q:a /c:"msiexec.exe /qb /i vcredist.msi"', '', SW_SHOW, ewWaitUntilTerminated, ResultCode)

[InstallDelete]
Type: files; Name: {app}\biusb.dll
Type: files; Name: {app}\f4im.url

[UninstallDelete]
Type: files; Name: {app}\usim.url

[Code]
procedure DoPreInstall();
var
  ResultCode: Integer;

begin
Log('Inside DoPreInstall');
ExtractTemporaryFile('vcredist_x86.exe');
Exec(ExpandConstant('{tmp}\vcredist_x86.exe'), '/Q:a /c:"msiexec.exe /i vcredist.msi /qn"', '', SW_SHOW, ewWaitUntilTerminated, ResultCode)
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    DoPreInstall();
  end;
end;