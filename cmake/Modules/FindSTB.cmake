# FindSTB - attempts to locate the stb single-file public domain
#                                  libraries for C/C++
#
# This module defines the following variables (on success):
#   STB_INCLUDE_DIRS  - where to find stb_image.h
#   STB_FOUND         - if the library was successfully located
#
# It is trying a few standard installation locations, but can be customized
# with the following variables:
#   STB_ROOT_DIR      - root directory of a stb installation
#                       Headers are expected to be found in either:
#                       <STB_ROOT_DIR>/stb_image.h           OR
#                       <STB_ROOT_DIR>/include/stb_image.h
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
SET(_stb_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include")

# check environment variable
SET(_stb_ENV_ROOT_DIR "$ENV{STB_ROOT_DIR}")

IF(NOT STB_ROOT_DIR AND _stb_ENV_ROOT_DIR)
    SET(STB_ROOT_DIR "${_stb_ENV_ROOT_DIR}")
ENDIF(NOT STB_ROOT_DIR AND _stb_ENV_ROOT_DIR)

# put user specified location at beginning of search
IF(STB_ROOT_DIR)
    SET(_stb_HEADER_SEARCH_DIRS "${STB_ROOT_DIR}"
                                "${STB_ROOT_DIR}/include"
                                 ${_stb_HEADER_SEARCH_DIRS})
ENDIF(STB_ROOT_DIR)

# locate header
FIND_PATH(STB_INCLUDE_DIR "stb_image.h"
    PATHS ${_stb_HEADER_SEARCH_DIRS})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(STB DEFAULT_MSG
    STB_INCLUDE_DIR)

IF(STB_FOUND)
    SET(STB_INCLUDE_DIRS "${STB_INCLUDE_DIR}")

    MESSAGE(STATUS "STB_INCLUDE_DIR = ${STB_INCLUDE_DIR}")
ENDIF(STB_FOUND)
