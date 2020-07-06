# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# 使用する言語を設定
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")

# プロジェクト名を設定
project(03_ShowObj)

add_definitions(-DPATH_ROOT_DIR="${PROJECT_SOURCE_DIR}/..")

############

# glfw
set(GLFW_LIBRARY
  ../3rd_party/glfw/src/libglfw3.a)
get_filename_component(GLFW_LIBRARY ${GLFW_LIBRARY} ABSOLUTE)
set(GLFW_INCLUDE_DIR
  ../3rd_party/glfw/include)

# OpenGL
find_package(OpenGL REQUIRED)

############

include_directories(
  ${GLFW_INCLUDE_DIR}
  ../3rd_party)

# ShowObjという実行ファイルをmain.cppから作成
add_executable(${PROJECT_NAME}
  main.cpp
  ../3rd_party/objloader.cpp)

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
