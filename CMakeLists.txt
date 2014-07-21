cmake_minimum_required (VERSION 2.6)

project(DumbFramework)

option(BUILD_TESTS "Build unit tests." OFF)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if(MSVC)
    add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

find_package(EXPAT)
find_package(SOIL)
find_package(Threads)
find_package(OpenGL)
find_package(GLEW)
find_package(GLFW)
find_package(GLM)
find_package(BOX2D)
find_package(AntTweakBar)
find_package(Freetype-gl)

set(DUMB_FRAMEWORK_VERSION 0.1a)
set(PROJECT_NAME DumbFramework)

include_directories( ${EXPAT_INCLUDE_DIR} 
                     ${PTHREADS_INCLUDE_DIR}
                     ${OPENGL_INCLUDE_DIRS}
                     ${GLFW_INCLUDE_DIR}
                     ${GLEW_INCLUDE_DIRS}
                     ${GLM_INCLUDE_DIRS}
                     ${SOIL_INCLUDE_DIRS}
                     ${FREETYPE_GL_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     src/)

set(DUMB_FRAMEWORK_SOURCES
    src/application.cpp
    src/boundingbox.cpp
    src/boundingsphere.cpp
    src/boundingfrustum.cpp
    src/boundingcircle.cpp
    src/boundingquad.cpp
    src/line2d.cpp
    src/plane.cpp
    src/ray.cpp
    src/bufferobject.cpp
    src/file.cpp
    src/module.cpp
    src/severity.cpp
    src/log.cpp
    src/shader.cpp
    src/program.cpp
    src/sprengine.cpp
    src/sprite.cpp
    src/texture2d.cpp
    src/textureloader.cpp
    src/windowhint.cpp)

add_library(DumbFramework STATIC ${DUMB_FRAMEWORK_SOURCES})
target_link_libraries(DumbFramework ${OPENGL_LIBRARIES} )
target_link_libraries(DumbFramework ${GLFW_LIBRARIES} )
target_link_libraries(DumbFramework ${GLEW_LIBRARY} )
target_link_libraries(DumbFramework ${PTHREADS_LIBRARY} )
target_link_libraries(DumbFramework ${SOIL_LIBRARIES} )
target_link_libraries(DumbFramework ${EXPAT_LIBRARY} )
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
        src/test/runtests.cpp)
    
    add_executable(RunTests ${DUMB_FRAMEWORK_TEST_SOURCES})
    target_link_libraries(RunTests DumbFramework)
    target_link_libraries(RunTests ${UNITTEST++_LIBRARY})
endif()

add_executable(flatengine src/demo/flatengine.cpp)
target_link_libraries(flatengine DumbFramework)

add_executable(vbo src/demo/vbo.cpp)
target_link_libraries(vbo DumbFramework)

