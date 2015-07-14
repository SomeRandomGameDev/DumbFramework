cmake_minimum_required (VERSION 2.6)
include(CheckCXXSymbolExists)

project(wopr)
set(PROJECT_NAME wopr)

# Force build type to Debug if none was specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE EQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../../cmake/")
set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(ARCHIVE_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

add_definitions(-DGLM_FORCE_RADIANS)

if(MSVC)
    add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

set(GLM_MINIMUM_VERSION "0.9.5.4")

find_package(tinyxml2)
find_package(SOIL)
find_package(Threads)
find_package(OpenGL)
find_package(GLEW)
find_package(GLFW)
find_package(GLM ${GLM_MINIMUM_VERSION} REQUIRED)
find_package(ICU 4.4 REQUIRED)
find_package(Doxygen)

include_directories( ${EXPAT_INCLUDE_DIR} 
                     ${TINYXML2_INCLUDE_DIR} 
                     ${PTHREADS_INCLUDE_DIR}
                     ${OPENGL_INCLUDE_DIRS}
                     ${GLFW_INCLUDE_DIR}
                     ${GLEW_INCLUDE_DIRS}
                     ${GLM_INCLUDE_DIRS}
                     ${SOIL_INCLUDE_DIRS}
                     ${FREETYPE_GL_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_SOURCE_DIR}/../../include/
                     ${CMAKE_SOURCE_DIR}/../../external/)

link_directories( ${DUMB_FRAMEWORK_LIBPATH} )

add_custom_target( resources ALL
    COMMAND rsync -r "${CMAKE_SOURCE_DIR}/resources" "${EXECUTABLE_OUTPUT_PATH}/")

add_executable(wopr wopr.cpp board.cpp)
add_dependencies(resources wopr)
target_link_libraries(wopr DumbFramework 
                           ${TINYXML2_LIBRARY}
                           ${OPENGL_LIBRARIES}
                           ${GLFW_LIBRARIES}
                           ${GLEW_LIBRARY}
                           ${PTHREADS_LIBRARY}
                           ${SOIL_LIBRARIES}
                           ${THREAD_LIBRARY}
                           ${FREETYPE_GL_LIBRARY}
                           ${CMAKE_THREAD_LIBS_INIT}
                           ${ICU_LIBRARIES} )
