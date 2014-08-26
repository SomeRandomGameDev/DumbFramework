# Locate tinyxml2
# This module defines
#  TINYXML2_FOUND, if false, do not try to link to tinyxml2
#  TINYXML2_LIBRARIES / TINYXML2_LIBRARY
#  TINYXML2_INCLUDE_DIR

if (TINYXML2_LIBRARIES AND TINYXML2_INCLUDE_DIR)
  # in cache already
  set(TINYXML2_FOUND TRUE)
else (TINYXML2_LIBRARIES AND TINYXML2_INCLUDE_DIR)

find_path(TINYXML2_INCLUDE_DIR
    NAMES tinyxml2.h
    PATH_SUFFIXES include Include
    PATHS
        /usr
        /usr/local
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
)

find_library(TINYXML2_LIBRARY
    NAMES tinyxml2
    PATH_SUFFIXES lib lib64 Library
    PATHS
        /usr
        /usr/local
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
)

if (TINYXML2_LIBRARY)
  set(TINYXML2_LIBRARIES ${TINYXML2_LIBRARIES} ${TINYXML2_LIBRARY})
endif (TINYXML2_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TINYXML2 DEFAULT_MSG TINYXML2_LIBRARY TINYXML2_INCLUDE_DIR)

mark_as_advanced(TINYXML2_INCLUDE_DIR TINYXML2_LIBRARY TINYXML2_LIBRARIES)

endif (TINYXML2_LIBRARIES AND TINYXML2_INCLUDE_DIR)

