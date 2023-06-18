cmake_minimum_required(VERSION 2.8)
option(USE_HEADERONLY "USE_DFM2_HEADERONLY" ON)
project(OpenGL_Examples)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(00_HelloWorld)
add_subdirectory(01_openwin)
add_subdirectory(01_openwinNewGlad)
add_subdirectory(02_ShowTexture)
add_subdirectory(03_ShowObj)
add_subdirectory(04_ShowObjWithShader)
add_subdirectory(05_ScreenShot)
add_subdirectory(06_ProgrammableShader)
