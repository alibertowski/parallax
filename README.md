# Parallax
A 2D game engine (for now) made with C++ using Vulkan and soon OpenGL made just for fun (and learning).

## Prerequisites
* [Vulkan](https://www.vulkan.org/tools#download-these-essential-development-tools)
* [GLFW](https://www.glfw.org/download)
* GLSL Compiler (I used glslangValidator)

## Installing
### Linux
1. Run make
2. WIP

### Windows - Using MSVC 2019
1. Download the required dependencies
2. Install the libraries someone on your system
3. Open the MSVC Build tools CMD and cd into this project
4. Run the following command after replacing the directories with the directories that you installed the libraries
`cl.exe /O2 /EHsc /nologo /DNDEBUG /W4 /std:c++17 /MD /GS /FeC:\Users\Alex\source\repos\parallax\bin\parallax.exe /FoC:\Users\Alex\source\repos\parallax\build\ /I"C:\Users\Alex\Documents\Visual Studio 2019\Libraries\glm" /I"C:\Users\Alex\Documents\Visual Studio 2019\Libraries\glfw-3.3.5.bin.WIN64\include" /IC:\Users\Alex\source\repos\parallax\lib\glad\include /IC:\Users\Alex\source\repos\parallax\lib\stb /IC:\Users\Alex\source\repos\parallax\include src\*.cpp lib/glad/src/glad.c /link /NOLOGO /MACHINE:X64 /LIBPATH:"C:\Users\Alex\Documents\Visual Studio 2019\Libraries\glfw-3.3.5.bin.WIN64\lib-vc2019" glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib`
5. Run
