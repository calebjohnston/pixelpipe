#================================================================================
# Fluid Dynamics Engine CMake file
#================================================================================

cmake_minimum_required(VERSION 2.6)
# set(CMAKE_BUILD_TYPE Debug)

if(APPLE AND UNIX)
  set(LIB_PATH "/usr/local/lib")
  set(INC_PATH "/usr/local/include")
elseif(UNIX AND NOT APPLE)
  set(LIB_PATH "/usr/local/lib")
  set(INC_PATH "/usr/local/include")
elseif(WIN32)
  set(LIB_PATH "C:/msys/1.0/local/lib")
  set(INC_PATH "C:/msys/1.0/local/include")
endif(APPLE AND UNIX)

#--------------------------------------------------------------------------------
# We need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
include_directories (
  # Look in the cmake build directory (some generated headers could be there)
  ${INC_PATH}
  ${CMAKE_CURRENT_BINARY_DIR} 
)

#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
# set( EXECUTABLE_OUTPUT_PATH ../../bin/tests )
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/tests)

add_executable( ocl_test
  main.c
)



## Link libraries
find_package(OpenCL REQUIRED)
target_link_libraries(ocl_test ${OPENCL_LIBRARIES})

file(COPY example.cl DESTINATION ${EXECUTABLE_OUTPUT_PATH})