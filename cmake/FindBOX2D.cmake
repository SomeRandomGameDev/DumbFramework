# Locate Box2D
# This module defines
#  BOX2D_FOUND, if false, do not try to link to Box2D
#  BOX2D_LIBRARIES / BOX2D_LIBRARY
#  BOX2D_INCLUDE_DIR

if (BOX2D_LIBRARIES AND BOX2D_INCLUDE_DIR)
  # in cache already
  set(BOX2D_FOUND TRUE)
else (BOX2D_LIBRARIES AND BOX2D_INCLUDE_DIR)

find_path(BOX2D_INCLUDE_DIR
    NAMES Box2D.h
    PATH_SUFFIXES include Include include/Box2D
    PATHS
        /usr
        /usr/local
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)

find_library(BOX2D_LIBRARY
	NAMES Box2D
    PATH_SUFFIXES lib lib64 Library
	PATHS
		/usr
		/usr/local
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)

if (BOX2D_LIBRARY)
  set(BOX2D_LIBRARIES ${BOX2D_LIBRARIES} ${BOX2D_LIBRARY})
endif (BOX2D_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOX2D DEFAULT_MSG BOX2D_LIBRARY BOX2D_INCLUDE_DIR)

mark_as_advanced(BOX2D_INCLUDE_DIR BOX2D_LIBRARY BOX2D_LIBRARIES)

endif (BOX2D_LIBRARIES AND BOX2D_INCLUDE_DIR)

