# - Find plib
# Find the native plib includes and library
#
#  PLIB_INCLUDE_DIRS - where to find plib.h
#  PLIB_LIBRARIES    - List of libraries when using plib.
#  PLIB_FOUND        - True if plib found.

IF(PLIB_INCLUDE_DIR AND PLIB_LIBRARY)
  # Already in cache, be silent
  SET(PLIB_FIND_QUIETLY TRUE)
ENDIF(PLIB_INCLUDE_DIR AND PLIB_LIBRARY)

FIND_PATH(PLIB_INCLUDE_DIR plib.h
          PATHS "${PLIB_DIR}"
          PATH_SUFFIXES include
          )

FIND_LIBRARY(PLIB_LIBRARY NAMES plib plib-0
             PATHS "${PLIB_DIR}"
             PATH_SUFFIXES lib
             )

# handle the QUIETLY and REQUIRED arguments and set PLIB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Plib DEFAULT_MSG PLIB_LIBRARY PLIB_INCLUDE_DIR)

IF(PLIB_FOUND)
    #SET(PLIB_LIBRARIES ${PLIB_LIBRARY})
    SET(PLIB_LIBRARIES -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc ) # hardcode instead!
    SET(PLIB_INCLUDE_DIRS ${PLIB_INCLUDE_DIR})
ENDIF(PLIB_FOUND)
