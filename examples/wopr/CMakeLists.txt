cmake_minimum_required (VERSION 2.8)
include(CheckCXXSymbolExists)

project(wopr)
set(WOPR_MAJOR "0")
set(WOPR_MINOR "1")
set(WOPR_PATCH "0")
set(VERSION "${WOPR_MAJOR}.${WOPR_MINOR}.${WOPR_PATCH}")
set(PROJECT_NAME wopr)
set(PACKAGE wopr)
set(PACKAGE_VERSION "${VERSION}")
add_definitions(-DPACKAGE_VERSION="${PACKAGE_VERSION}")

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

if(GENERATE_PACKAGE_TARGET)
    add_definitions(-DBUILD_PACKAGE)
endif(GENERATE_PACKAGE_TARGET)

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

install(TARGETS wopr DESTINATION "games/")
install(FILES "${EXECUTABLE_OUTPUT_PATH}/resources/atlas.png" DESTINATION "share/wopr/")

if(GENERATE_PACKAGE_TARGET)
    include(InstallRequiredSystemLibraries)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "wopr is a simple tic-tac-toe game")
    set(CPACK_PACKAGE_VENDOR "Stoned Xander and MooZ")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${WOPR_MAJOR}.${WOPR_MINOR}.${WOPR_PATCH}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${WOPR_MAJOR}.${WOPR_MINOR}.${WOPR_PATCH}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGE_VERSION_MAJOR ${WOPR_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${WOPR_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${WOPR_PATCH})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "wopr")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_STRIP_FILES "bin/wopr")
    set(CPACK_PACKAGE_EXECUTABLES "wopr" "wopr")
    set(CPACK_SOURCE_IGNORE_FILES "/CVS/" "/.svn/" "/.git/" "build/" "installers/" "/CMakeLists.txt.user$" "~$" "\\\\.swp$" "\\\\.#" "/#")
    set(CPACK_RPM_PACKAGE_LICENSE "Apache License, Version 2.0")
    set(CPACK_RPM_PACKAGE_GROUP "Amusements/Graphics")
    set(CPACK_RPM_PACKAGE_URL "http://blockos.org/")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <contact@blockos.org>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${VERSION}+deb1")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${CPACK_RPM_PACKAGE_URL}")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libtinyxml2-2, libglew1.7, libglfw3 (>= 3.0.4), libicu52, libc6 (>= 2.14), libstdc++6 (>= 4.9)")
    set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
    include(CPack)
endif()
