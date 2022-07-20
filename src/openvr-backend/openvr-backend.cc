#include "openvr-backend.hh"

#include <openvr/openvr.h>
#include <wayland-server-core.h>

#include <iostream>

#include "backend.h"
#include "util.h"

namespace ovrwl {

OpenVRBackend::OpenVRBackend() {}

bool
OpenVRBackend::Connect()
{
  vr::EVRInitError init_error = vr::VRInitError_None;

  vr_system_ = vr::VR_Init(&init_error, vr::VRApplication_Scene);
  if (init_error != vr::VRInitError_None) {
    std::cerr << "Failed to init OpenVR: "
              << vr::VR_GetVRInitErrorAsEnglishDescription(init_error)
              << std::endl;
    goto err;
  }

  return true;

err:
  return false;
}

void
OpenVRBackend::Disconnect()
{
  vr::VR_Shutdown();
  vr_system_ = nullptr;
}

bool
OpenVRBackend::Render()
{
  vr::TrackedDevicePose_t
      tracked_device_post_list[vr::k_unMaxTrackedDeviceCount];
  vr::VREvent_t event;

  vr_system_->PollNextEvent(&event, sizeof(event));

  switch (event.eventType) {
    case vr::VREvent_Quit:
    case vr::VREvent_ProcessQuit:
    case vr::VREvent_DriverRequestedQuit:
      fprintf(stderr, "quit event: %d\n", event.eventType);
      return false;
  }

  vr::VRCompositor()->WaitGetPoses(
      tracked_device_post_list, vr::k_unMaxTrackedDeviceCount, NULL, 0);

  return true;
}

OpenVRBackend::~OpenVRBackend() { vr::VR_Shutdown(); }

}  // namespace ovrwl

struct backend {
  ovrwl::OpenVRBackend *openvr_backend;
  struct wl_event_source *repaint_timer;
};

int
backend_repaint(void *data)
{
  struct backend *self = (backend *)data;
  if (self->openvr_backend->Render() == false) {
    self->openvr_backend->Disconnect();
    return false;
  }
  wl_event_source_timer_update(self->repaint_timer, 5);
  fprintf(stderr, ".");
  return 0;
}

bool
backend_connect(struct backend *self)
{
  if (self->openvr_backend->Connect() == false) return false;

  wl_event_source_timer_update(self->repaint_timer, 5);

  return true;
}

struct backend *
backend_create(struct wl_display *display)
{
  struct backend *self;
  struct wl_event_loop *loop = wl_display_get_event_loop(display);

  self = (backend *)zalloc(sizeof *self);
  if (self == NULL) {
    std::cerr << "No memory" << std::endl;
    goto err;
  }

  self->openvr_backend = new ovrwl::OpenVRBackend();
  self->repaint_timer = wl_event_loop_add_timer(loop, backend_repaint, self);

  return self;

err:
  return NULL;
}

void
backend_destroy(struct backend *self)
{
  delete self->openvr_backend;
  free(self);
}
