::/EHsc command-line option instructs the compiler to enable C++ exception handling
::To create an executable file, you must invoke LINK:
::LINK firsti.obj second.obj third.obj /OUT:filename.exe


::ECHO fileextension=%%~xi
::ECHO filepath=%%~pi
::ECHO filedrive=%%~di
::ECHO filename=%%~ni
@echo off
set FREEGLUT_LIB=glut-win-libs\freeglut\lib\freeglut.lib
set GLEWLIB_RELEASE_GLEW32=glut-win-libs\glew-1.10.0\lib\Release\Win32\glew32.lib
set CURRENT_GLUT=%FREEGLUT_LIB%
set CURRENT_GLEW=%GLEWLIB_RELEASE_GLEW32%
set FREEGLUT_INCLUDE=glut-win-libs\freeglut\include
set GLEW_INCLUDE=glut-win-libs\glew-1.10.0\include
set BUILD_DIR=build\
set ASTOX_OBJS=%BUILD_DIR%Value.obj %BUILD_DIR%String.obj %BUILD_DIR%Boolean.obj %BUILD_DIR%Number.obj %BUILD_DIR%Object.obj %BUILD_DIR%Array.obj %BUILD_DIR%Function.obj %BUILD_DIR%Lang.obj %BUILD_DIR%SScript.obj %BUILD_DIR%Pins.obj %BUILD_DIR%FileSystem.obj %BUILD_DIR%Xhtml.obj %BUILD_DIR%Query.obj
set GLEXT_OBJS=%BUILD_DIR%Shapes.obj %BUILD_DIR%Shapes3d.obj %BUILD_DIR%Interfaces.obj %BUILD_DIR%Algorithms.obj %BUILD_DIR%Window.obj

echo "hello world"


cl /clr filesystemclr.cpp

::ASTOX_OBJS
::for /r %%i in (src\astox\*.cpp) do ( 
	::CL /EHsc /Fobuild\%%~ni.obj %%i
::)

::LOMBARDI_OBJS
::for /r %%i in (src\lombardi\*Win.cpp) do ( 
	::CL /EHsc /Fobuild\%%~ni.obj %%i
::)



::GLEXT_OBJS
::for /r %%i in (src\glext\*.cpp) do ( 
	::CL /I%FREEGLUT_INCLUDE% /I%GLEW_INCLUDE%  /EHsc %CURRENT_GLUT% %CURRENT_GLEW% /Fobuild\%%~ni.obj %%i 
::)

::glext-tests
::cl /I%FREEGLUT_INCLUDE% /I%GLEW_INCLUDE% /Fe%BUILD_DIR%\glext-tests /EHsc src\glext-tests.cpp %ASTOX_OBJS% %GLEXT_OBJS% %CURRENT_GLUT% %CURRENT_GLEW%


::sample_fylesystem
::cl /Fe%BUILD_DIR%\sample_filesystem /EHsc src\sample_filesystem.cpp %ASTOX_OBJS% 

:: create automatic obj files, dummy.obj is ignored
:: CL /EHsc /c src\astox\String.cpp dummy.obj 

:: THIS IS OBJ MAKE
::CL /EHsc /Fosrc\astox\String.obj src\astox\String.cpp 