#ifndef OPENVR_WITH_WAYLAND_OPENVR_BACKEND_H
#define OPENVR_WITH_WAYLAND_OPENVR_BACKEND_H

#include <openvr/openvr.h>

namespace ovrwl {

class OpenVRBackend
{
 public:
  OpenVRBackend();
  ~OpenVRBackend();

  bool Connect();
  void Disconnect();
  bool Render();

 private:
  vr::IVRSystem *vr_system_;
  vr::IVRCompositor *vr_compositor_;
};

}  // namespace ovrwl

#endif  //  OPENVR_WITH_WAYLAND_OPENVR_BACKEND_H
