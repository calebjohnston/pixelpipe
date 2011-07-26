#================================================================================
# Pipeline CMake file
#================================================================================

cmake_minimum_required( VERSION 2.6 )
project( Pipeline )
set( CMAKE_BUILD_TYPE Debug)

if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/)
else("${CMAKE_GENERATOR}" STREQUAL "XCode")
  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
  set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
endif("${CMAKE_GENERATOR}" STREQUAL "Xcode")

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)

include( CMakeForceCompiler )
# cmake_force_cxx_compiler( g++ g++ )
cmake_force_cxx_compiler( clang++ Clang++ )

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
