@echo off
echo "Currently this only builds an optimized version"
echo "Compiling and linking"
echo "-----------------------------------"

set user=Temp
set glfw-include-path=C:\Users\%user%\Documents\Visual Studio 2019\Libraries\glfw-3.3.5.bin.WIN64\include
set glfw-lib-path=C:\Users\%user%\Documents\Visual Studio 2019\Libraries\glfw-3.3.5.bin.WIN64\lib-vc2019
set glm-include-path=C:\Users\%user%\Documents\Visual Studio 2019\Libraries\glm

set project-path=C:\Users\%user%\source\repos\parallax

md build
md bin

cl.exe /O2 /EHsc /nologo /DNDEBUG /W4 /std:c++17 /MD /GS /Fe%project-path%\bin\parallax.exe /Fo%project-path%\build\ /I"%glm-include-path%" /I"%glfw-include-path%" /I%project-path%\lib\glad\include /I%project-path%\lib\stb /I%project-path%\include src\*.cpp src\opengl\*.cpp lib/glad/src/glad.c /link /NOLOGO /MACHINE:X64 /LIBPATH:"%glfw-lib-path%" glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
pause
