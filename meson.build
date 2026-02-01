project('clap_app', 'c',
  version: '0.1.0',
  default_options: [
    'c_std=c11',
    'buildtype=release',
  ],
  meson_version: '>= 0.54.0',
)

# Compilers
cc = meson.get_compiler('c')

# Platform detection
host_system = host_machine.system()
is_windows = host_system == 'windows'
is_darwin = host_system == 'darwin'
is_linux = host_system == 'linux' or (not is_windows and not is_darwin)

# Platform-specific defines
platform_defines = []
if is_darwin
  platform_defines += ['-DIS_MAC=1']
elif is_linux
  platform_defines += ['-DIS_LINUX=1']
elif is_windows
  platform_defines += ['-DIS_WIN=1']
endif

# Find dependencies
cairo_dep = dependency('cairo', required: true)
opengl_dep = dependency('gl', required: true)

# Platform-specific dependencies
platform_deps = []
platform_link_args = []
pugl_extra_deps = []

if is_windows
  dwmapi_dep = cc.find_library('dwmapi')
  d3d12_dep = cc.find_library('d3d12')
  dxgi_dep = cc.find_library('dxgi')
  d3dcompiler_dep = cc.find_library('d3dcompiler')
  dxguid_dep = cc.find_library('dxguid')
  platform_deps += [d3d12_dep, dxgi_dep, d3dcompiler_dep, dxguid_dep]
  pugl_extra_deps += [dwmapi_dep]
elif is_darwin
  add_languages('objc', native: false)
  platform_link_args += [
    '-framework', 'CoreFoundation',
    '-framework', 'AppKit',
    '-framework', 'CoreGraphics',
  ]
endif

# CLAP headers (interface library equivalent)
clap_inc = include_directories('external/clap/include')

# Pugl - build from external directory
pugl_inc = include_directories('external/pugl/include')

# Platform-specific pugl source
if is_darwin
  pugl_platform_source = files('external/pugl/src/mac.m')
elif is_windows
  pugl_platform_source = files('external/pugl/src/win.c')
else
  pugl_platform_source = files('external/pugl/src/x11.c')
endif

pugl_sources = files(
  'external/pugl/src/common.c',
  'external/pugl/src/internal.c',
) + pugl_platform_source

pugl_lib = static_library('pugl',
  pugl_sources,
  include_directories: pugl_inc,
  dependencies: [cairo_dep, opengl_dep] + pugl_extra_deps,
  c_args: ['-DPUGL_STATIC'],
)

pugl_dep = declare_dependency(
  link_with: pugl_lib,
  include_directories: pugl_inc,
  compile_args: ['-DPUGL_STATIC'],
)

# Include directories
project_inc = include_directories('include')

# Copy headers custom target
lib_headers = files(
  'lib/app.h',
  'lib/clap-adapters.h',
  'lib/clap-features-adapters.h',
  'lib/clap-gui-adapters.h',
  'lib/dsp.h',
  'lib/gui.h',
)

# Create include/cdsp directory and copy headers
# Use a simple copy command
python = find_program('python3', 'python', required: true)

copy_headers = custom_target('copy_headers',
  input: lib_headers,
  output: [
    'app.h',
    'clap-adapters.h',
    'clap-features-adapters.h',
    'clap-gui-adapters.h',
    'dsp.h',
    'gui.h',
  ],
  command: [
    python, '-c',
    'import shutil, os, sys; dst = os.path.join(r"@SOURCE_ROOT@", "include", "cdsp"); os.makedirs(dst, exist_ok=True); [shutil.copy(f, dst) for f in sys.argv[1:]]',
    '@INPUT@',
  ],
  build_by_default: true,
)

# Library sources
lib_sources = files(
  'lib/app.c',
  'lib/clap-adapters.c',
  'lib/clap-features-adapters.c',
  'lib/clap-gui-adapters.c',
  'lib/gui.c',
  'lib/empty.c',
)

# cdsp-lib static library
cdsp_lib = static_library('cdsp-lib',
  lib_sources,
  include_directories: [clap_inc, pugl_inc],
  dependencies: [cairo_dep, opengl_dep, pugl_dep],
  c_args: ['-DPUGL_STATIC'],
)

cdsp_dep = declare_dependency(
  link_with: cdsp_lib,
  include_directories: [project_inc, clap_inc, pugl_inc],
  dependencies: [cairo_dep, opengl_dep, pugl_dep],
  compile_args: ['-DPUGL_STATIC'],
)

# Plugin sources (excluding main.c)
plugin_sources = files(
  'src/minimal_plugin.c',
)

# Main plugin module (.clap)
clap_plugin = shared_module('clap_app',
  plugin_sources,
  include_directories: [project_inc, pugl_inc],
  dependencies: [cdsp_dep, cairo_dep, opengl_dep] + platform_deps,
  c_args: platform_defines + ['-DPUGL_STATIC'],
  link_args: platform_link_args,
  name_prefix: '',
  name_suffix: 'clap',
  install: true,
)

# Standalone executable
standalone_sources = files(
  'src/main.c',
)

standalone_exe = executable('clap_app_standalone',
  standalone_sources,
  include_directories: [project_inc, clap_inc, include_directories('src'), pugl_inc],
  dependencies: [cdsp_dep, cairo_dep, opengl_dep] + platform_deps,
  c_args: platform_defines,
  link_args: platform_link_args,
  install: true,
)

# Optional: copy plugin after build
copy_after_build = get_option('copy_after_build')
if copy_after_build
  if is_darwin
    install_dir = '~/Library/Audio/Plug-Ins/CLAP'
  elif is_linux
    install_dir = '~/.clap'
  else
    install_dir = ''
  endif

  if install_dir != ''
    message('Will copy plugin to @0@ after build'.format(install_dir))
  endif
endif

# Summary
summary({
  'Platform': host_system,
  'Cairo': cairo_dep.found(),
  'OpenGL': opengl_dep.found(),
}, section: 'Configuration')
