cmake_minimum_required(VERSION 2.8)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

IF(MSVC)
ELSE()
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
ENDIF()

project(03_ShowObj)

add_definitions(-DPATH_ROOT_DIR="${PROJECT_SOURCE_DIR}/..")

############

# glfw
IF(MSVC)
  set(GLFW_LIBRARY    ../3rd_party/glfw/src/Release/glfw3.lib)
ELSE()
  set(GLFW_LIBRARY    ../3rd_party/glfw/src/libglfw3.a)
ENDIF()
get_filename_component(GLFW_LIBRARY ${GLFW_LIBRARY} ABSOLUTE)
set(GLFW_INCLUDE_DIR
  ../3rd_party/glfw/include)

# OpenGL
find_package(OpenGL REQUIRED)

############

set(3RD_PARTY_DIR ../3rd_party)

include_directories(
  ${GLFW_INCLUDE_DIR}
  ${3RD_PARTY_DIR}
  ../src)

add_executable(${PROJECT_NAME}
  main.cpp
  ../src/objloader.cpp)

###########

IF(APPLE)
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
ELSEIF(UNIX)
  target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp)
ELSEIF(MSVC)
  target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY})
ENDIF()
