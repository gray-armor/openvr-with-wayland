_scanner_path = wayland_scanner_dep.get_variable('wayland_scanner')
_wayland_scanner = find_program(_scanner_path)


_protocols = [
  'zen-desktop.xml',
]

_wayland_protocol_srcs = []
_wayland_protocol_server_headers = []
_wayland_protocol_client_headers = []

foreach xml : _protocols
  _wayland_protocol_srcs += custom_target(
    xml.underscorify() + '_server_c',
    input: xml,
    output: '@BASENAME@-protocol.c',
    command: [_wayland_scanner, 'private-code', '@INPUT@', '@OUTPUT@'],
  )
  _wayland_protocol_client_headers += custom_target(
    xml.underscorify() + '_client_h',
    input: xml,
    output: '@BASENAME@-client-protocol.h',
    command: [_wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
  )
  _wayland_protocol_server_headers += custom_target(
    xml.underscorify() + '_server_h',
    input: xml,
    output: '@BASENAME@-protocol.h',
    command: [_wayland_scanner, 'server-header', '@INPUT@', '@OUTPUT@'],
  )
endforeach

_lib_server_protocols = static_library(
  'server_protocols',
  _wayland_protocol_srcs + _wayland_protocol_server_headers,
  dependencies: wayland_server_dep.partial_dependency(compile_args: true)
)

server_protocols = declare_dependency(
  link_with: _lib_server_protocols,
  sources: _wayland_protocol_server_headers,
)

_lib_client_protocols = static_library(
  'client_protocols',
  _wayland_protocol_srcs + _wayland_protocol_client_headers,
  dependencies: wayland_client_dep.partial_dependency(compile_args: true)
)

client_protocols = declare_dependency(
  link_with: _lib_client_protocols,
  sources: _wayland_protocol_client_headers,
)
