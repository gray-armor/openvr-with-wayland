#ifndef OPENVR_WITH_WAYLAND_INTERNAL_H
#define OPENVR_WITH_WAYLAND_INTERNAL_H

#include <wayland-server-core.h>

#include "backend.h"
#include "util.h"
#include "zen-desktop-protocol.h"

struct mode_switcher {
  struct wl_global *global;
  struct wl_display *display;
  struct wl_list resources;  // list of wl_resource of zen_desktop_mode_switcher

  struct wl_signal switch_mode_signal;
};

struct mode_switcher *mode_switcher_create(struct wl_display *display);

void mode_switcher_destroy(struct mode_switcher *self);

#endif  //  OPENVR_WITH_WAYLAND_INTERNAL_H
