# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# 使用する言語を設定
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11) # version 11 以降のC++を使う宣言
set(CMAKE_CXX_STANDARD_REQUIRED TRUE) # C++ のstdライブラリを使う宣言
set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
# warningをどのくらい出すか -Wall->全てwarningを出す, -Wno-dep..->deprecatedは出さない。

# プロジェクト名を設定
project(04_ShowObjWithShader)

add_definitions(-DPATH_ROOT_DIR="${PROJECT_SOURCE_DIR}/..")

############

# glfw
set(GLFW_LIBRARY
  ../3rd_party/glfw/src/libglfw3.a)
get_filename_component(GLFW_LIBRARY ${GLFW_LIBRARY} ABSOLUTE)
set(GLFW_INCLUDE_DIR
  ../3rd_party/glfw/include)
  
# glad
set(GLAD_INCLUDE_DIR "../3rd_party/glad/include")
set(GLAD_SRC_DIR "../3rd_party/glad/src")
get_filename_component(GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} ABSOLUTE)
get_filename_component(GLAD_SRC_DIR ${GLAD_SRC_DIR} ABSOLUTE)

# OpenGL
find_package(OpenGL REQUIRED)

############

include_directories(
  ${GLFW_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIR}
  ../3rd_party)

# ShowObjという実行ファイルをmain.cppから作成
add_executable(${PROJECT_NAME}
  ${GLAD_INCLUDE_DIR}/glad/glad.h
  ${GLAD_SRC_DIR}/glad.c
  ../3rd_party/objloader.cpp
  main.cpp)

###########

find_library( COCOA_LIBRARY Cocoa )
find_library( IOKIT_LIBRARY IOKit )
find_library( COREVIDEO_LIBRARY CoreVideo )
target_link_libraries(${PROJECT_NAME}
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARY}
  ${COCOA_LIBRARY}
  ${IOKIT_LIBRARY}
  ${COREVIDEO_LIBRARY}
)
