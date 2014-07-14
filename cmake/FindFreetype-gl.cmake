# Locate freetype-gl
# This module defines
#  FREETYPE_GL_FOUND, if false, do not try to link to freetype-gl
#  FREETYPE_GL_LIBRARIES / FREETYPE_GL_LIBRARY
#  FREETYPE_GL_INCLUDE_DIR

if (FREETYPE_GL_LIBRARIES AND FREETYPE_GL_INCLUDE_DIR)
  # in cache already
  set(FREETYPE_GL_FOUND TRUE)
else (FREETYPE_GL_LIBRARIES AND FREETYPE_GL_INCLUDE_DIR)

find_path(FREETYPE_GL_INCLUDE_DIR
    NAMES freetype-gl.h
    PATH_SUFFIXES include Include include/freetype-gl
    PATHS
        /usr
        /usr/local
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)

find_library(FREETYPE_GL_LIBRARY
	NAMES freetype-gl
    PATH_SUFFIXES lib lib64 Library
	PATHS
		/usr
		/usr/local
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
)

if (FREETYPE_GL_LIBRARY)
  set(FREETYPE_GL_LIBRARIES ${FREETYPE_GL_LIBRARIES} ${FREETYPE_GL_LIBRARY})
endif (FREETYPE_GL_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FREETYPE_GL DEFAULT_MSG FREETYPE_GL_LIBRARY FREETYPE_GL_INCLUDE_DIR)

mark_as_advanced(FREETYPE_GL_INCLUDE_DIR FREETYPE_GL_LIBRARY FREETYPE_GL_LIBRARIES)

endif (FREETYPE_GL_LIBRARIES AND FREETYPE_GL_INCLUDE_DIR)

