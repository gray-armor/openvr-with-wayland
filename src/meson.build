inc_priv = include_directories('.')

subdir('openvr-backend')

_sources = [
  'main.c',
  'mode-switcher.c',
]

_dependencies = [
  backend_dep,
  server_protocols,
  wayland_server_dep,
]

executable(
  'openvr-wayland',
  _sources,
  install: false,
  dependencies: _dependencies,
)

executable(
  'switch',
  'switch.c',
  install: false,
  dependencies: [ client_protocols, wayland_client_dep ],
)
