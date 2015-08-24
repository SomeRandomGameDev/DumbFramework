cmake_minimum_required (VERSION 2.6)
include(CheckCXXSymbolExists)

project(DumbFramework)

set(DUMB_FW_MAJOR_VERSION 0)
set(DUMB_FW_MINOR_VERSION 0)
set(DUMB_FW_PATCH_VERSION 1)
set(DUMB_FW_VERSION "${DUMB_FW_MAJOR_VERSION}.${DUMB_FW_MINOR_VERSION}.${DUMB_FW_PATCH_VERSION}")

# Force build type to Debug if none was specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

option(BUILD_TESTS "Build unit tests." OFF)
option(SANITY_CHECK "Enable sanity checks. WARNING: performance loss + large logs." OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE EQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

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
find_package(ICU 4.4 REQUIRED)
find_package(Doxygen)
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

set(DUMB_FRAMEWORK_GEOMETRY_SOURCES
    src/geometry/boundingbox.cpp
    src/geometry/boundingcircle.cpp
    src/geometry/boundingquad.cpp
    src/geometry/boundingsphere.cpp
    src/geometry/frustum.cpp
    src/geometry/line2.cpp
    src/geometry/ray.cpp
    src/geometry/rect.cpp
    src/geometry/plane.cpp
    src/geometry/transform.cpp
    src/geometry/camera.cpp)

set(DUMB_FRAMEWORK_RENDER_SOURCES
    src/render/renderer.cpp
    src/render/bufferobject.cpp
    src/render/uniformbuffer.cpp
    src/render/geometry.cpp
    src/render/vertexstream.cpp
    src/render/shader.cpp
    src/render/program.cpp
    src/render/rendercontext.cpp
    src/render/renderbuffer.cpp
    src/render/texture2d.cpp
    src/render/textureloader.cpp)

set(DUMB_FRAMEWORK_SOURCES
    ${DUMB_FRAMEWORK_GEOMETRY_SOURCES}
    ${DUMB_FRAMEWORK_RENDER_SOURCES}
    src/imguidelegate.cpp
    src/file.cpp
    src/module.cpp
    src/severity.cpp
    src/log.cpp
    src/render.cpp
    src/sprengine.cpp
    src/sprite.cpp
    src/adviser.cpp
    src/font.cpp)

if(MSVC)
set(DUMB_FRAMEWORK_SOURCES
    ${DUMB_FRAMEWORK_SOURCES}
    src/platform/msvc/file.cpp)
endif(MSVC)

if(UNIX)
set(DUMB_FRAMEWORK_SOURCES
    ${DUMB_FRAMEWORK_SOURCES}
    src/platform/unix/file.cpp)
endif(UNIX)

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
target_link_libraries(DumbFramework ${ICU_LIBRARIES})


if(BUILD_TESTS)
    find_package(UnitTest++)

    include_directories( ${UNITTEST++_INCLUDE_DIR} )

    set(DUMB_FRAMEWORK_TEST_SOURCES
        src/test/boundingbox.cpp
        src/test/boundingsphere.cpp
        src/test/frustum.cpp
        src/test/boundingcircle.cpp
        src/test/plane.cpp
        src/test/log.cpp
        src/test/transform.cpp
        src/test/runtests.cpp)
    
    add_executable(RunTests ${DUMB_FRAMEWORK_TEST_SOURCES})
    target_link_libraries(RunTests DumbFramework)
    target_link_libraries(RunTests ${UNITTEST++_LIBRARY})
endif()

add_custom_target( resources ALL
#    COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}"
    COMMAND rsync -r "${CMAKE_SOURCE_DIR}/resources" "${EXECUTABLE_OUTPUT_PATH}/")

add_executable(flatengine src/demo/flatengine.cpp)
add_dependencies(resources flatengine)
target_link_libraries(flatengine DumbFramework)

add_executable(font-basic src/demo/font/basic.cpp)
add_dependencies(resources font-basic)
target_link_libraries(font-basic DumbFramework)

set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY IMMEDIATE)

add_custom_target(doc
         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
         SOURCES ${DOXYFILE}
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         COMMENT "Generating API documentation with Doxygen"
         VERBATIM )
