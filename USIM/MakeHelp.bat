@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by USIM.HPJ. >"hlp\USIM.hm"
echo. >>"hlp\USIM.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\USIM.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\USIM.hm"
echo. >>"hlp\USIM.hm"
echo // Prompts (IDP_*) >>"hlp\USIM.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\USIM.hm"
echo. >>"hlp\USIM.hm"
echo // Resources (IDR_*) >>"hlp\USIM.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\USIM.hm"
echo. >>"hlp\USIM.hm"
echo // Dialogs (IDD_*) >>"hlp\USIM.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\USIM.hm"
echo. >>"hlp\USIM.hm"
echo // Frame Controls (IDW_*) >>"hlp\USIM.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\USIM.hm"
REM -- Make help for Project USIM


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\USIM.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\USIM.hlp" goto :Error
if not exist "hlp\USIM.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\USIM.hlp" Debug
if exist Debug\nul copy "hlp\USIM.cnt" Debug
if exist Release\nul copy "hlp\USIM.hlp" Release
if exist Release\nul copy "hlp\USIM.cnt" Release
echo.
goto :done

:Error
echo hlp\USIM.hpj(1) : error: Problem encountered creating help file

:done
echo.
