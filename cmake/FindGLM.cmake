# FindGLM - attempts to locate the glm matrix/vector library.
#
# This module defines the following variables (on success):
#   GLM_INCLUDE_DIRS  - where to find glm/glm.hpp
#   GLM_FOUND         - if the library was successfully located
#
# It is trying a few standard installation locations, but can be customized
# with the following variables:
#   GLM_ROOT_DIR      - root directory of a glm installation
#                       Headers are expected to be found in either:
#                       <GLM_ROOT_DIR>/glm/glm.hpp           OR
#                       <GLM_ROOT_DIR>/include/glm/glm.hpp
#                       This variable can either be a cmake or environment
#                       variable. Note however that changing the value
#                       of the environment varible will NOT result in
#                       re-running the header search and therefore NOT
#                       adjust the variables set by this module.

#=============================================================================
# Copyright 2012 Carsten Neumann
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# default search dirs
SET(_glm_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include")

# check environment variable
SET(_glm_ENV_ROOT_DIR "$ENV{GLM_ROOT_DIR}")

IF(NOT GLM_ROOT_DIR AND _glm_ENV_ROOT_DIR)
    SET(GLM_ROOT_DIR "${_glm_ENV_ROOT_DIR}")
ENDIF(NOT GLM_ROOT_DIR AND _glm_ENV_ROOT_DIR)

# put user specified location at beginning of search
IF(GLM_ROOT_DIR)
    SET(_glm_HEADER_SEARCH_DIRS "${GLM_ROOT_DIR}"
                                "${GLM_ROOT_DIR}/include"
                                 ${_glm_HEADER_SEARCH_DIRS})
ENDIF(GLM_ROOT_DIR)

# locate header
FIND_PATH(GLM_INCLUDE_DIR "glm/glm.hpp" 
    PATHS ${_glm_HEADER_SEARCH_DIRS})

if(GLM_INCLUDE_DIR)
    SET(GLM_VERSION_MAJOR 0)
    SET(GLM_VERSION_MINOR 0)
    SET(GLM_VERSION_PATCH 0)
    SET(GLM_VERSION_REVISION 0)
    
    SET(GLM_VERSION_FILE "${GLM_INCLUDE_DIR}/glm/detail/setup.hpp")
    if(EXISTS "${GLM_VERSION_FILE}")
        # Read and parse glm setup header file for version number
        file(READ "${GLM_VERSION_FILE}" _glm_HEADER_CONTENTS)
        string(REGEX REPLACE ".*#define GLM_VERSION_MAJOR[ \t]+([0-9]+).*" "\\1" GLM_VERSION_MAJOR "${_glm_HEADER_CONTENTS}")
        string(REGEX REPLACE ".*#define GLM_VERSION_MINOR[ \t]+([0-9]+).*" "\\1" GLM_VERSION_MINOR "${_glm_HEADER_CONTENTS}")
        string(REGEX REPLACE ".*#define GLM_VERSION_PATCH[ \t]+([0-9]+).*" "\\1" GLM_VERSION_PATCH "${_glm_HEADER_CONTENTS}")
        string(REGEX REPLACE ".*#define GLM_VERSION_REVISION[ \t]+([0-9]+).*" "\\1" GLM_VERSION_REVISION "${_glm_HEADER_CONTENTS}")
    endif(EXISTS "${GLM_VERSION_FILE}")

    SET(GLM_VERSION_STRING "${GLM_VERSION_MAJOR}.${GLM_VERSION_MINOR}.${GLM_VERSION_PATCH}.${GLM_VERSION_REVISION}")
endif(GLM_INCLUDE_DIR)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM
    REQUIRED_VARS GLM_INCLUDE_DIR
    VERSION_VAR GLM_VERSION_STRING)

IF(GLM_FOUND)
    SET(GLM_INCLUDE_DIRS "${GLM_INCLUDE_DIR}")
    MESSAGE(STATUS "GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIR}")
ENDIF(GLM_FOUND)
