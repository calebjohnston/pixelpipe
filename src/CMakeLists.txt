#================================================================================
# Pipeline CMake file
#================================================================================

cmake_minimum_required(VERSION 2.6)
set(CMAKE_BUILD_TYPE Debug)
if("${CMAKE_BUILD_TYPE}" EQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE TRUE) # this does not even work...
endif()

set(BOOST_LIBS thread date_time system program_options)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

if(APPLE AND UNIX)
  set(LIB_PATH "/usr/local/lib" ${PROJECT_SOURCE_DIR}/lib)
  set(INC_PATH "/usr/local/include")
elseif(UNIX AND NOT APPLE)
  set(LIB_PATH "/usr/local/lib" ${PROJECT_SOURCE_DIR}/lib)
  set(INC_PATH "/usr/local/include")
elseif(WIN32)
  set(LIB_PATH "C:/msys/1.0/local/lib" ${PROJECT_SOURCE_DIR}/lib)
  set(INC_PATH "C:/msys/1.0/local/include")
endif(APPLE AND UNIX)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
set( pipeline_SRCS
  core/main.cpp
  core/camera.cpp
  # core/clipper.cpp
  # core/framebuffer.cpp
  # core/pipeline.cpp
  # core/rasterizer.cpp
  # core/texture.cpp
  # logger/logger.cpp
  # logger/logwriter.cpp
  # logger/stdiowriter.cpp
  # logger/syslogwriter.cpp
  # vertex/vert_color.cpp
  # vertex/vert_frag_shaded.cpp
  # vertex/vert_frag_shaded.cpp
  # vertex/vert_frag_textured.cpp
  # vertex/vert_shaded_processor.cpp
  # fragment/frag_color.cpp
  # fragment/frag_phong.cpp
  # fragment/frag_textured.cpp
  # fragment/frag_zbuffer.cpp
  # vecmath/color.cpp
  # vecmath/mat2.cpp
  # vecmath/mat3.cpp
  # vecmath/mat4.cpp
  # vecmath/quat.cpp
  # vecmath/symmat2.cpp
  # vecmath/symmat3.cpp
  # vecmath/symmat4.cpp
)

set(CG_LIBRARIES
  ${PROJECT_SOURCE_DIR}/lib/libcg_image.dylib 
  ${PROJECT_SOURCE_DIR}/lib/libcg_vecmath.dylib
)

#--------------------------------------------------------------------------------
include_directories (
  ${INC_PATH}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PNG_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${TIFF_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/extern
  ${PROJECT_SOURCE_DIR}/include
)

# how can I setup an automated versioning that can be reflected in source code?
set( TARGET_VERSION_MAJOR 0 )
set( TARGET_VERSION_MINOR 2 )
add_definitions( -DCMAKE_TARGET_VERSION=1 -DTARGET_VERSION_MAJOR=${TARGET_VERSION_MAJOR} -DTARGET_VERSION_MINOR=${TARGET_VERSION_MINOR})

add_executable( pipeline
  ${pipeline_SRCS}
)


#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
target_link_libraries ( pipeline
  ${Boost_LIBRARIES}
  ${CG_LIBRARIES}
  ${PNG_LIBRARY}
  ${JPEG_LIBRARY}
  ${TIFF_LIBRARY}
  ${GLUT_LIBRARY}
  ${OPENGL_LIBRARY}
)