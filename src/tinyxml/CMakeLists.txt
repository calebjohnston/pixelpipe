#================================================================================
# PixelPipe CMake file
#================================================================================

cmake_minimum_required(VERSION 2.6)

#--------------------------------------------------------------------------------
# Set the "-I" flags.
#--------------------------------------------------------------------------------
include_directories( ${PROJECT_SOURCE_DIR}/extern )

#--------------------------------------------------------------------------------
# Additional compiler flags; theoretically this should be added to the 
# cache in the top-level CMakeLists.txt file.
#--------------------------------------------------------------------------------
add_definitions( -DTIXML_USE_STL )

#--------------------------------------------------------------------------------
# Associate source dependencies to target(s).
#--------------------------------------------------------------------------------
add_library( tinyxml SHARED 
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
)
