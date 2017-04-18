::/EHsc command-line option instructs the compiler to enable C++ exception handling
::To create an executable file, you must invoke LINK:
::LINK firsti.obj second.obj third.obj /OUT:filename.exe

@echo off


setlocal enabledelayedexpansion

set MAIN_FILE=main.cpp
set MAIN_OBJ=main.obj
set UNITTEST_FILE=
set ASTOX_DIR=src\astox
set BUILD_DIR=D:\Dropbox\astox\build
set BIN_DIR=D:\Dropbox\astox\bin

del %BUILD_DIR%\*obj
del %BUILD_DIR%\*pdb
set OBJS=%BUILD_DIR%\MinGUI.obj^
		%BUILD_DIR%\Array.obj^
		%BUILD_DIR%\Boolean.obj^
		%BUILD_DIR%\CssLinter.obj^
		%BUILD_DIR%\Date.obj^
		%BUILD_DIR%\EcmaMethods.obj^
		%BUILD_DIR%\ECMAScript.obj^
		%BUILD_DIR%\Object.obj^
		%BUILD_DIR%\Number.obj^
		%BUILD_DIR%\Musteen.obj^
		%BUILD_DIR%\EventDispatcher.obj^
		%BUILD_DIR%\Pins.obj^
		%BUILD_DIR%\Properties.obj^
		%BUILD_DIR%\Operators.obj^
		%BUILD_DIR%\FileSystem.obj^
		%BUILD_DIR%\Query.obj^
		%BUILD_DIR%\SelfConfigurer.obj^
		%BUILD_DIR%\Socket.obj^
		%BUILD_DIR%\ExtApi.obj^
		%BUILD_DIR%\String.obj^
		%BUILD_DIR%\Function.obj^
		%BUILD_DIR%\StringAlg.obj^
		%BUILD_DIR%\SScript.obj^
		%BUILD_DIR%\HTTPServer.obj^
		%BUILD_DIR%\Thread.obj^
		%BUILD_DIR%\Xhtml.obj^
		%BUILD_DIR%\Lang.obj^
		%BUILD_DIR%\Value.obj^
		%BUILD_DIR%\SScriptData.obj^
		%BUILD_DIR%\%MAIN_OBJ%
set CPPS=%ASTOX_DIR%\MinGUI.cpp^
	     %ASTOX_DIR%\SScript.cpp^
		 %ASTOX_DIR%\SScriptData.cpp^
	     %ASTOX_DIR%\Thread.cpp^
	     %ASTOX_DIR%\Value.cpp^
	     %ASTOX_DIR%\Array.cpp^
	     %ASTOX_DIR%\Socket.cpp^
	     %ASTOX_DIR%\Object.cpp^
	     %ASTOX_DIR%\StringAlg.cpp^
	     %ASTOX_DIR%\Xhtml.cpp^
	     %ASTOX_DIR%\String.cpp^
	     %ASTOX_DIR%\SelfConfigurer.cpp^
	     %ASTOX_DIR%\Query.cpp^
	     %ASTOX_DIR%\Musteen.cpp^
	     %ASTOX_DIR%\Operators.cpp^
	     %ASTOX_DIR%\Number.cpp^
	     %ASTOX_DIR%\Boolean.cpp^
	     %ASTOX_DIR%\Properties.cpp^
	     %ASTOX_DIR%\Pins.cpp^
	     %ASTOX_DIR%\CssLinter.cpp^
	     %ASTOX_DIR%\Date.cpp^
	     %ASTOX_DIR%\EcmaMethods.cpp^
	     %ASTOX_DIR%\ECMAScript.cpp^
	     %ASTOX_DIR%\Lang.cpp^
	     %ASTOX_DIR%\HTTPServer.cpp^
	     %ASTOX_DIR%\ExtApi.cpp^
	     %ASTOX_DIR%\FileSystem.cpp^
	     %ASTOX_DIR%\Function.cpp^
	     %ASTOX_DIR%\EventDispatcher.cpp
set CL_COMMAND=/c /Zi /W3 /WX- /O2 /Oi /Oy- /GL /D WIN32 /D ASTOX_SOCK_WIN /D NDEBUG /D _CONSOLE /D _UNICODE /D UNICODE /Gm- /EHsc /MD /GS /Gy /fp:precise /Zc:wchar_t /Zc:forScope /Fo"build\\" /Fd"build\vc100.pdb" /Gd /TP /analyze- /errorReport:prompt
set LINK_COMMAND=/ERRORREPORT:PROMPT /OUT:%BIN_DIR%\astox.exe /INCREMENTAL:NO /NOLOGO ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /MANIFEST /ManifestFile:%BIN_DIR%\astox.exe.intermediate.manifest /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEBUG /PDB:"%BUILD_DIR%\astox.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"%BUILD_DIR%\astox.lib" /MACHINE:X86 %OBJS%
cl %CPPS% src\%MAIN_FILE% %CL_COMMAND%
LINK %LINK_COMMAND%
