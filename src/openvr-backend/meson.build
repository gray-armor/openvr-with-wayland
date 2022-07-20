_backend_srcs = [
  'openvr-backend.cc',
]

_backend_deps = [
  openvr_dep,
]

_backend_lib = static_library(
  'openvr-backend',
  _backend_srcs,
  install: false,
  include_directories: inc_priv,
  dependencies: _backend_deps,
)

backend_dep = declare_dependency(
  link_with: _backend_lib,
)
