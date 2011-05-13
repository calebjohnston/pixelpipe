#================================================================================
# Pipeline CMake file
#================================================================================

cmake_minimum_required( VERSION 2.6 )
project( Pipeline )
set( CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)


# include( cmake/modules/FindOpenCL.cmake )
set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)
set( Pipeline_BUILD_TESTS OFF CACHE BOOL "Build tests")
set( Pipeline_BUILD_DOCS ON CACHE BOOL "Build documentation")

# add_subdirectory( lib )
add_subdirectory( src )
if(Pipeline_BUILD_TESTS)
  add_subdirectory( tests )
  add_subdirectory( tests/openCL )
endif(Pipeline_BUILD_TESTS)

add_custom_target( tests DEPENDS xml_handling threads windowing ocl_test pcg_test io_logger )
# add_custom_target( stuff DEPENDS Pipeline )

# Add the Doxyfile.in and UseDoxygen.cmake files to the projects source directory.
if(Pipeline_BUILD_DOCS)
  set( DOXYFILE_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/docs )
  set( DOXYFILE_LATEX "NO" )
  include( UseDoxygen )
endif(Pipeline_BUILD_DOCS)
# SET( DOXYFILE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )

#
#  DOXYFILE_SOURCE_DIR - Path where the Doxygen input files are.
#  	Defaults to the current source and binary directory.
#  DOXYFILE_OUTPUT_DIR - Path where the Doxygen output is stored. Defaults to "doc".
#
#  DOXYFILE_LATEX - Set to "NO" if you do not want the LaTeX documentation
#  	to be built.
#  DOXYFILE_LATEX_DIR - Directory relative to DOXYFILE_OUTPUT_DIR where
#  	the Doxygen LaTeX output is stored. Defaults to "latex".
#
#  DOXYFILE_HTML_DIR - Directory relative to DOXYFILE_OUTPUT_DIR where
#  	the Doxygen html output is stored. Defaults to "html".
#

