include(ExternalProject)

cmake_minimum_required(VERSION 3.16)
project(clap_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH /usr/local)

set(CMAKE_FIND_DEBUG_MODE OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(cairo REQUIRED)

find_package(OpenGL REQUIRED)

add_library(clap INTERFACE)
target_include_directories(clap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/clap/include)

file(GLOB SOURCES
  "src/*.c"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

file(GLOB LIBSOURCES 
  "lib/*.c"
)

file(GLOB LIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h")

add_custom_target(copy_headers
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/cdsp
)

foreach(header ${LIB_HEADERS})
  add_custom_command(TARGET copy_headers POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${header} ${CMAKE_SOURCE_DIR}/include/cdsp/
  )
endforeach()

find_package(cmocka CONFIG REQUIRED)

# Enable testing
enable_testing()

# Pugl vendored directory
set(PUGL_DIR "${CMAKE_SOURCE_DIR}/external/pugl")

# Add Pugl include directory to search path
set(PUGL_INCLUDE_DIR "${PUGL_DIR}/include")

# Select platform implementation
if(APPLE)
  set(PUGL_PLATFORM_SOURCE "${PUGL_DIR}/src/mac.m")
  set(PUGL_BACKEND_SOURCES "${PUGL_DIR}/src/mac_gl.m")
elseif(WIN32)
  set(PUGL_PLATFORM_SOURCE "${PUGL_DIR}/src/win.c")
  set(PUGL_BACKEND_SOURCES "${PUGL_DIR}/src/win_gl.c")
else()
  set(PUGL_PLATFORM_SOURCE "${PUGL_DIR}/src/x11.c")
  set(PUGL_BACKEND_SOURCES "${PUGL_DIR}/src/x11_gl.c")
endif()

set(PUGL_SOURCES
  ${PUGL_DIR}/src/common.c
  ${PUGL_DIR}/src/internal.c
  ${PUGL_PLATFORM_SOURCE}
  ${PUGL_BACKEND_SOURCES}
)

add_library(pugl STATIC ${PUGL_SOURCES})
target_include_directories(pugl PRIVATE
  ${PUGL_INCLUDE_DIR}
)
target_compile_definitions(pugl PUBLIC PUGL_STATIC)

target_link_libraries(pugl cairo::cairo OpenGL::GL)

if(WIN32)
  target_link_libraries(pugl dwmapi)
endif()

add_library(cdsp-lib STATIC ${LIBSOURCES})
target_include_directories(cdsp-lib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/external/clap/include
  ${PUGL_INCLUDE_DIR}
  ${cairo_INCLUDE_DIR}
)
target_link_libraries(cdsp-lib PRIVATE cairo::cairo OpenGL::GL pugl)
add_dependencies(cdsp-lib copy_headers)
target_compile_definitions(cdsp-lib PUBLIC PUGL_STATIC)

add_library(${PROJECT_NAME} MODULE ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PUBLIC PUGL_STATIC)
target_link_libraries(${PROJECT_NAME} cdsp-lib clap cairo::cairo OpenGL::GL)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PUGL_INCLUDE_DIR}
  ${cairo_INCLUDE_DIR}
)

# Standalone executable target
add_executable(${PROJECT_NAME}_standalone src/main.c)
target_compile_definitions(${PROJECT_NAME}_standalone PRIVATE)
target_link_libraries(${PROJECT_NAME}_standalone cdsp-lib clap cairo::cairo OpenGL::GL )
target_include_directories(${PROJECT_NAME}_standalone PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/clap/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PUGL_INCLUDE_DIR}
)

if(APPLE)
  target_link_libraries(${PROJECT_NAME}_standalone "-framework CoreFoundation" "-framework AppKit" "-framework CoreGraphics")
  target_compile_definitions(${PROJECT_NAME}_standalone PRIVATE IS_MAC=1)
elseif(UNIX)
  target_compile_definitions(${PROJECT_NAME}_standalone PRIVATE IS_LINUX=1)
elseif(WIN32)
  target_link_libraries(${PROJECT_NAME}_standalone d3d12.lib dxgi.lib d3dcompiler.lib dxguid.lib)
  target_compile_definitions(${PROJECT_NAME}_standalone PRIVATE IS_WIN=1)
endif()

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    BUNDLE True
    BUNDLE_EXTENSION clap
    MACOSX_BUNDLE_GUI_IDENTIFIER org.surge-synth-team.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}.plist.in
  )
  target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation" "-framework AppKit" "-framework CoreGraphics")

  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_MAC=1)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror
    $<$<BOOL:${USE_SANITIZER}>:-fsanitize=address>
    $<$<BOOL:${USE_SANITIZER}>:-fsanitize=undefined>
  )
  target_link_options(${PROJECT_NAME} PRIVATE
    $<$<BOOL:${USE_SANITIZER}>:-fsanitize=address>
    $<$<BOOL:${USE_SANITIZER}>:-fsanitize=undefined>
  )

  if (${COPY_AFTER_BUILD})
    message(STATUS "Will copy plugin after every build" )
    set(products_folder ${CMAKE_BINARY_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E echo "Installing ${products_folder}/${PROJECT_NAME}.clap to ~/Library/Audio/Plug-Ins/CLAP/"
      COMMAND ${CMAKE_COMMAND} -E make_directory "~/Library/Audio/Plug-Ins/CLAP"
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${products_folder}/${PROJECT_NAME}.clap" "~/Library/Audio/Plug-Ins/CLAP/${PROJECT_NAME}.clap"
    )
  endif()
elseif(UNIX)
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_LINUX=1)
  target_sources(${PROJECT_NAME} PRIVATE src/linux-vstgui-adapter.cpp)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".clap" PREFIX "")
  if (${COPY_AFTER_BUILD})
    message(STATUS "Will copy plugin after every build" )
    set(products_folder ${CMAKE_BINARY_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E echo "Installing ${products_folder}/${PROJECT_NAME}.clap to ~/.clap"
      COMMAND ${CMAKE_COMMAND} -E make_directory "~/.clap"
      COMMAND ${CMAKE_COMMAND} -E copy "${products_folder}/${PROJECT_NAME}.clap" "~/.clap"
    )
  endif()

else()
  target_link_libraries(${PROJECT_NAME} d3d12.lib dxgi.lib d3dcompiler.lib dxguid.lib)
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_WIN=1)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".clap" PREFIX "")
endif()

# ============================================================================
# Unit Tests (cmocka)
# ============================================================================

# Common test include directories and link libraries
set(TEST_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/clap/include
  ${PUGL_INCLUDE_DIR}
  ${cairo_INCLUDE_DIR}
)

# Test: app module
add_executable(test_app tests/test_app.c)
target_include_directories(test_app PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(test_app PRIVATE cdsp-lib cmocka::cmocka)
target_compile_definitions(test_app PRIVATE PUGL_STATIC)
add_test(NAME test_app COMMAND test_app)

# Test: dsp module
add_executable(test_dsp tests/test_dsp.c)
target_include_directories(test_dsp PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(test_dsp PRIVATE cdsp-lib cmocka::cmocka)
target_compile_definitions(test_dsp PRIVATE PUGL_STATIC)
add_test(NAME test_dsp COMMAND test_dsp)

# Test: clap adapters
add_executable(test_clap_adapters tests/test_clap_adapters.c)
target_include_directories(test_clap_adapters PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(test_clap_adapters PRIVATE cdsp-lib cmocka::cmocka)
target_compile_definitions(test_clap_adapters PRIVATE PUGL_STATIC)
add_test(NAME test_clap_adapters COMMAND test_clap_adapters)

# Test: clap features adapters
add_executable(test_clap_extensions tests/test_clap_extensions_adapters.c)
target_include_directories(test_clap_extensions PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(test_clap_extensions PRIVATE cdsp-lib cmocka::cmocka)
target_compile_definitions(test_clap_extensions PRIVATE PUGL_STATIC)
add_test(NAME test_clap_extensions COMMAND test_clap_extensions)

# Test: clap gui adapters
add_executable(test_clap_gui_adapters tests/test_clap_gui_adapters.c)
target_include_directories(test_clap_gui_adapters PRIVATE ${TEST_INCLUDE_DIRS})
target_link_libraries(test_clap_gui_adapters PRIVATE cdsp-lib cmocka::cmocka)
target_compile_definitions(test_clap_gui_adapters PRIVATE PUGL_STATIC)
if(WIN32)
  target_compile_definitions(test_clap_gui_adapters PRIVATE IS_WIN=1)
elseif(APPLE)
  target_compile_definitions(test_clap_gui_adapters PRIVATE IS_MAC=1)
else()
  target_compile_definitions(test_clap_gui_adapters PRIVATE IS_LINUX=1)
endif()
add_test(NAME test_clap_gui_adapters COMMAND test_clap_gui_adapters)
