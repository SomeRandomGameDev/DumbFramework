cmake_minimum_required (VERSION 2.6)

project(DumbFramework)

option(BUILD_TESTS "Build unit tests." OFF)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

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

set(DUMB_FRAMEWORK_VERSION 0.1a)
set(PROJECT_NAME DumbFramework)
set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})

include_directories( ${EXPAT_INCLUDE_DIR} 
                     ${PTHREADS_INCLUDE_DIR}
                     ${OPENGL_INCLUDE_DIRS}
                     ${GLFW_INCLUDE_DIR}
                     ${GLEW_INCLUDE_DIRS}
                     ${GLM_INCLUDE_DIRS}
                     ${SOIL_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     src/)

set(DUMB_FRAMEWORK_SOURCES
    src/application.cpp
    src/boundingsphere.cpp
    src/bufferobject.cpp
    src/file.cpp
    src/log.cpp
    src/sprengine.cpp
    src/sprite.cpp
    src/windowhint.cpp)

add_library(DumbFramework STATIC ${DUMB_FRAMEWORK_SOURCES})
target_link_libraries(DumbFramework ${OPENGL_LIBRARIES} )
target_link_libraries(DumbFramework ${GLFW_LIBRARIES} )
target_link_libraries(DumbFramework ${GLEW_LIBRARY} )
target_link_libraries(DumbFramework ${PTHREADS_LIBRARY} )
target_link_libraries(DumbFramework ${SOIL_LIBRARIES} )
target_link_libraries(DumbFramework ${EXPAT_LIBRARY} )
target_link_libraries(DumbFramework ${THREAD_LIBRARY} )
target_link_libraries(DumbFramework ${CMAKE_THREAD_LIBS_INIT})

if(BUILD_TESTS)
    find_package(UnitTest++)

    include_directories( ${UNITTEST++_INCLUDE_DIR} )

    set(DUMB_FRAMEWORK_TEST_SOURCES
        src/test/boundingsphere.cpp
        src/test/runtests.cpp)
    
    add_executable(RunTests ${DUMB_FRAMEWORK_TEST_SOURCES} ${DUMB_FRAMEWORK_SOURCES})
    target_link_libraries(RunTests DumbFramework)
    target_link_libraries(RunTests ${UNITTEST++_LIBRARY})
endif()

add_executable(flatengine src/demo/flatengine.cpp)
target_link_libraries(flatengine DumbFramework)
