cmake_minimum_required (VERSION 2.6)

project(DumbFramework)

option(BUILD_TESTS "Build unit tests." OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(MSVC)
    add_definitions (/D "_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

find_package(pthreads)
find_package(opengl)
find_package(glew 1.8.0 REQUIRED)
find_package(glfw)
find_package(glm)

set(DUMB_FRAMEWORK_VERSION 0.1a)
set(PROJECT_NAME DumbFramework)
set(EXECUTABLE_OUTPUT_PATH build/${CMAKE_BUILD_TYPE})

include_directories( ${PTHREADS_INCLUDE_DIR}
                     ${OPENGL_INCLUDE_DIRS}
                     ${GLFW_INCLUDE_DIR}
                     ${GLEW_INCLUDE_DIRS}
                     ${GLM_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     src/)

set(DUMB_FRAMEWORK_SOURCES
    src/boundingsphere.cpp
    src/file.cpp
    src/log.cpp)

add_library(DumbFramework STATIC ${DUMB_FRAMEWORK_SOURCES})

if(BUILD_TESTS)
    find_package(UnitTest++)

    include_directories( ${UNITTEST++_INCLUDE_DIR} )

    set(DUMB_FRAMEWORK_TEST_SOURCES
        src/test/boundingsphere.cpp
        src/test/runtests.cpp)
    
    add_executable(RunTests ${DUMB_FRAMEWORK_TEST_SOURCES} ${DUMB_FRAMEWORK_SOURCES})
    target_link_libraries(RunTests ${PTHREADS_LIBRARY} ${UNITTEST++_LIBRARY})
endif()

# target_link_libraries(todo ${OPENGL_LIBRARIES} )
# target_link_libraries(todo ${GLFW_LIBRARIES} )
# target_link_libraries(todo ${GLEW_LIBRARIES} )