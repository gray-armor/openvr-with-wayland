project(
  'openvr-with-wayland', 'c', 'cpp',
  version: '0.0.1',
  meson_version: '>= 0.58.0',
  default_options: [ 'warning_level=3', 'werror=true', 'optimization=2', 'c_std=gnu11' ],
)

cc = meson.get_compiler('c')

global_args = []
global_args_maybe = [
  '-D_GNU_SOURCE',
  '-DWLR_USE_UNSTABLE',
  '-fvisibility=hidden',
]

foreach arg : global_args_maybe
  if cc.has_argument(arg)
    global_args += arg
  endif
endforeach
add_project_arguments(global_args, language: 'c')

# generic version requirements

openvr_req = '>= 1.12.5'
wayland_server_req = '>= 1.18.0'
wayland_client_req = '>= 1.18.0'

# dependencies

openvr_dep = dependency('openvr', version: openvr_req)
wayland_server_dep = dependency('wayland-server', version: wayland_server_req)
wayland_client_dep = dependency('wayland-client', version: wayland_client_req)
wayland_scanner_dep = dependency('wayland-scanner')

subdir('protocol')
subdir('src')
