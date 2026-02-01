file(GLOB VCPKG_TRIPLET_DIRS "${CMAKE_SOURCE_DIR}/vcpkg_installed/*")
foreach(TRIPLET_DIR ${VCPKG_TRIPLET_DIRS})
  if(IS_DIRECTORY ${TRIPLET_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${TRIPLET_DIR})
  endif()
endforeach()

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_path(cairo_INCLUDE_DIR
  NAMES cairo.h
  PATH_SUFFIXES cairo
)

find_library(cairo_LIBRARY
  NAMES cairo
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cairo
  REQUIRED_VARS cairo_LIBRARY cairo_INCLUDE_DIR
)


if(cairo_FOUND AND NOT TARGET cairo::cairo)
  add_library(cairo::cairo UNKNOWN IMPORTED)
  set_target_properties(cairo::cairo PROPERTIES
    IMPORTED_LOCATION "${cairo_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${cairo_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(cairo_INCLUDE_DIR cairo_LIBRARY)
