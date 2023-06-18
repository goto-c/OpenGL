cmake_minimum_required(VERSION 3.4)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

IF(MSVC)
ELSE()
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
ENDIF()

project(06_ProgrammableShader)

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
  
# glad
set(GLAD_INCLUDE_DIR "../3rd_party/glad/include")
set(GLAD_SRC_DIR "../3rd_party/glad/src")
get_filename_component(GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} ABSOLUTE)
get_filename_component(GLAD_SRC_DIR ${GLAD_SRC_DIR} ABSOLUTE)

# OpenGL
find_package(OpenGL REQUIRED)

# glm 
# find_package(glm REQUIRED)

# spdlog
set(SPDLOG_INCLUDE_DIR "../3rd_party/spdlog/include")

############

set(3RD_PARTY_DIR ../3rd_party)

include_directories(
  ${GLFW_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
  ${3RD_PARTY_DIR}/glm/
  ${3RD_PARTY_DIR}
  ../include)

add_executable(${PROJECT_NAME}
  ${GLAD_INCLUDE_DIR}/glad/glad.h
  ${GLAD_SRC_DIR}/glad.c
  ../src/objloader.cpp
  ../src/shader.cpp
  main.cpp)

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
