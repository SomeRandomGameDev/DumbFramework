cmake_minimum_required (VERSION 2.6)
include(CheckCXXSymbolExists)

project(DumbFramework)

# Force build type to Debug if none was specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


option(BUILD_TESTS "Build unit tests." OFF)
option(SANITY_CHECK "Enable sanity checks. WARNING: performance loss + large logs." OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(ARCHIVE_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

if(SANITY_CHECK)
    add_definitions(-DSANITY_CHECK)
endif(SANITY_CHECK)

add_definitions(-DGLM_FORCE_RADIANS)

if(MSVC)
    add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

set(GLM_MINIMUM_VERSION "0.9.5.4")

# find_package(EXPAT)
find_package(tinyxml2)
find_package(SOIL)
find_package(Threads)
find_package(OpenGL)
find_package(GLEW)
find_package(GLFW)
find_package(GLM ${GLM_MINIMUM_VERSION} REQUIRED)
# find_package(BOX2D)
# find_package(Freetype-gl)

set(DUMB_FRAMEWORK_VERSION 0.1a)
set(PROJECT_NAME DumbFramework)

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
                     include/
                     external/)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp)

set(DUMB_FRAMEWORK_RENDER_SOURCES
    src/render/renderer.cpp
    src/render/bufferobject.cpp
    src/render/geometry.cpp
    src/render/vertexstream.cpp
    src/render/shader.cpp
    src/render/program.cpp
    src/render/mesh.cpp
    src/render/material.cpp
    src/render/rendercontext.cpp
    src/render/renderbuffer.cpp
    src/render/texture2d.cpp
    src/render/textureloader.cpp)

set(DUMB_FRAMEWORK_SOURCES
    ${DUMB_FRAMEWORK_RENDER_SOURCES}
    src/camera.cpp
    src/imguidelegate.cpp
    src/application.cpp
    src/rect.cpp
    src/boundingbox.cpp
    src/boundingsphere.cpp
    src/boundingfrustum.cpp
    src/boundingcircle.cpp
    src/boundingquad.cpp
    src/line2d.cpp
    src/plane.cpp
    src/ray.cpp
    src/file.cpp
    src/module.cpp
    src/severity.cpp
    src/log.cpp
    src/sprengine.cpp
    src/sprite.cpp
    src/windowhint.cpp
    src/transform.cpp
    src/joystick.cpp
    src/mouse.cpp)

add_library(DumbFramework STATIC ${DUMB_FRAMEWORK_SOURCES} ${IMGUI_SOURCES})
target_link_libraries(DumbFramework ${TINYXML2_LIBRARY} )
target_link_libraries(DumbFramework ${OPENGL_LIBRARIES} )
target_link_libraries(DumbFramework ${GLFW_LIBRARIES} )
target_link_libraries(DumbFramework ${GLEW_LIBRARY} )
target_link_libraries(DumbFramework ${PTHREADS_LIBRARY} )
target_link_libraries(DumbFramework ${SOIL_LIBRARIES} )
target_link_libraries(DumbFramework ${THREAD_LIBRARY} )
target_link_libraries(DumbFramework ${FREETYPE_GL_LIBRARY} )
target_link_libraries(DumbFramework ${CMAKE_THREAD_LIBS_INIT})

if(BUILD_TESTS)
    find_package(UnitTest++)

    include_directories( ${UNITTEST++_INCLUDE_DIR} )

    set(DUMB_FRAMEWORK_TEST_SOURCES
        src/test/boundingbox.cpp
        src/test/boundingsphere.cpp
        src/test/boundingfrustum.cpp
        src/test/boundingcircle.cpp
        src/test/plane.cpp
        src/test/log.cpp
        src/test/transform.cpp
        src/test/runtests.cpp)
    
    add_executable(RunTests ${DUMB_FRAMEWORK_TEST_SOURCES})
    target_link_libraries(RunTests DumbFramework)
    target_link_libraries(RunTests ${UNITTEST++_LIBRARY})
endif()

add_executable(flatengine src/demo/flatengine.cpp)
target_link_libraries(flatengine DumbFramework)

add_executable(gui src/demo/gui.cpp)
target_link_libraries(gui DumbFramework)
