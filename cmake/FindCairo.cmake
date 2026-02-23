file(GLOB VCPKG_TRIPLET_DIRS "${CMAKE_BINARY_DIR}/vcpkg_installed/*")
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
  add_library(cairo::cairo STATIC IMPORTED GLOBAL)

  set(_base "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}")

  set_target_properties(cairo::cairo PROPERTIES
    IMPORTED_LOCATION "${_base}/lib/cairo.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${cairo_INCLUDE_DIR}"
  )

  target_link_libraries(cairo::cairo INTERFACE
    "${_base}/lib/pixman-1.lib"
    "${_base}/lib/freetype.lib"
    "${_base}/lib/fontconfig.lib"
    "${_base}/lib/libexpatMT.lib"
    "${_base}/lib/libpng16.lib"
    "${_base}/lib/zlib.lib"
    "${_base}/lib/bz2.lib"
    "${_base}/lib/brotlidec.lib"
    "${_base}/lib/brotlicommon.lib"
  )
endif()

mark_as_advanced(cairo_INCLUDE_DIR cairo_LIBRARY)
