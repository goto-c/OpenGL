# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)

# プロジェクト名と使用する言語を設定
project(02_ShowTexture CXX)

add_definitions(-DPATH_ROOT_DIR="${PROJECT_SOURCE_DIR}/..") # OpenGL directory

###########

# glfw
set(GLFW_LIBRARY    ../3rd_party/glfw/src/libglfw3.a)
get_filename_component(GLFW_LIBRARY ${GLFW_LIBRARY} ABSOLUTE)

# OpenGL
find_package(OpenGL REQUIRED)

###########

include_directories(
  ../3rd_party/glfw/include
  ../3rd_party)
#target_link_libraries(a.out ${GLFW_LIBRARY} ${OPENGL_LIBRARY})

# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(${PROJECT_NAME} main.cpp)

############

set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
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
