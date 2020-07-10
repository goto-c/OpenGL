cmake_minimum_required(VERSION 2.8)

project(02_ShowTexture CXX)

add_definitions(-DPATH_ROOT_DIR="${PROJECT_SOURCE_DIR}/..") # OpenGL directory

IF(MSVC)
ELSE()
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
ENDIF()

###########

# glfw
IF(MSVC)
  set(GLFW_LIBRARY    ../3rd_party/glfw/src/Release/glfw3.lib)
ELSE()
  set(GLFW_LIBRARY    ../3rd_party/glfw/src/libglfw3.a)
ENDIF()
get_filename_component(GLFW_LIBRARY ${GLFW_LIBRARY} ABSOLUTE)

# OpenGL
find_package(OpenGL REQUIRED)

###########

include_directories(
  ../3rd_party/glfw/include
  ../3rd_party)

add_executable(${PROJECT_NAME} main.cpp)

############

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
