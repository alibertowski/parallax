# Parallax
A 2D game engine (for now) made with C++ using Vulkan and soon OpenGL made just for fun (and learning).

## Prerequisites
* [Vulkan](https://vulkan.lunarg.com/sdk/home)
* [GLFW](https://www.glfw.org/download)
* GLSL Compiler (I used glslangValidator)

## Installing
### Linux
1. Run make
2. WIP

### Windows - Using VS 2019
1. Install the Vulkan SDK
2. Install the GLFW binaries and set them to your preferred directory (Default: Documents\Visual Studio 2019\Libraries\)
3. Open VS 2019, go to File->New->Project From Existing Code...
4. Select Visual C++, Next
5. Type in a project name and the location that the repository was cloned in, Next
6. Use Visual Studio->Windows application project, Next
7. Click Next for Debug Configuration settings
8. Uncheck 'Same as Debug Configuration' and set NDEBUG as a Preprocessor definition for Release configuration, Finish
9. Set project to 64 bit (x64)
10. Go to Project->Properties
11. Set 'Configuration' to 'All Configurations'
12. Go to C/C++->Language->C++ Language Standard, set it to C++17 or greater.
13. Go to C/C++->General->Additional Include Directories. Add the following directories:
  * Vulkan Include Directory: VulkanSDK\1.2.170.0\Include
  * GLFW Include Directory: glfw-3.3.4.bin.WIN64\include
  * Project Include Directory: [repository]\include
14. Go to Linker->General->Additional Library Directories. Add the following directories:
  * Vulkan Library Directory: VulkanSDK\1.2.170.0\Lib
  * GLFW Library Directory: glfw-3.3.4.bin.WIN64\lib-vc2019
15. Go to Linker->Input->Additional Dependencies. Add the following object files:
  * vulkan-1.lib
  * glfw3.lib
16. Build the project.
17. Create a folder called "shaders" in the same directory as the built exe (Unless the built exe is in the same directory as the already made "shaders" folder)
18. Compile the shaders in [repository]\shaders\ and place them in the newly created "shaders" folder
19. Run