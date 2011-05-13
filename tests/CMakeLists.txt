#================================================================================
# Pipeline CMake file
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
# set( EXECUTABLE_OUTPUT_PATH ../bin/tests )
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/tests)

add_executable( xml_handling
  xml_handling.cpp
)

add_executable( threads 
  threads.cpp
)

add_executable( windowing 
  windowing.c
)


## Link libraries
set(BOOST_LIBS thread date_time system program_options)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(xml_handling 
  ${Boost_LIBRARIES}
)

target_link_libraries(threads 
  ${Boost_LIBRARIES}
)

find_package(Threads REQUIRED)
target_link_libraries(xml_handling
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(threads 
  ${CMAKE_THREAD_LIBS_INIT}
)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(windowing 
  ${GLUT_LIBRARY}
  ${OPENGL_LIBRARY}
)

# this doesn't work anymore? What happened to debug_settings.xml
# file(COPY debug_settings.xml DESTINATION ${EXECUTABLE_OUTPUT_PATH})
