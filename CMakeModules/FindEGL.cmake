find_path(EGL_INCLUDE_DIR
  NAMES
    EGL/egl.h
  PATHS
    ${EGL_ROOT_DIR}/include
    /usr/local/include
    /usr/include)

find_library(EGL_LIBRARY
  NAMES
    EGL
  PATHS
    ${EGL_ROOT_DIR}/lib
    /usr/local/lib
    /usr/lib
    /usr/lib/x86_64-linux-gnu)

find_library(EGL_opengl_LIBRARY
  NAMES
   OpenGL
  PATHS
    ${EGL_ROOT_DIR}/lib
    /usr/local/lib
    /usr/lib
    /usr/lib/x86_64-linux-gnu)

set(EGL_LIBRARIES ${EGL_LIBRARY} ${EGL_opengl_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  EGL DEFAULT_MSG
  EGL_LIBRARY EGL_opengl_LIBRARY EGL_INCLUDE_DIR)
mark_as_advanced(EGL_ROOT_DIR EGL_INCLUDE_DIR EGL_LIBRARY EGL_opengl_LIBRARY)

if(EGL_FOUND)
  if(NOT TARGET EGL::OpenGL)
    add_library(EGL::OpenGL UNKNOWN IMPORTED)
    set_target_properties(EGL::OpenGL PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${EGL_INCLUDE_DIR}")
    set_target_properties(EGL::OpenGL PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      IMPORTED_LOCATION "${EGL_opengl_LIBRARY}")
  endif()

  if(NOT TARGET EGL::EGL)
    add_library(EGL::EGL UNKNOWN IMPORTED)
    set_target_properties(EGL::EGL PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${EGL_INCLUDE_DIR}")
    set_target_properties(EGL::EGL PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      INTERFACE_LINK_LIBRARIES "EGL::OpenGL"
      IMPORTED_LOCATION "${EGL_LIBRARY}")
  endif()
endif()