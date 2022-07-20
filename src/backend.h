#ifndef OPENVR_WITH_WAYLAND_BACKEND_H
#define OPENVR_WITH_WAYLAND_BACKEND_H

#include <stdbool.h>
#include <wayland-server-core.h>

#ifdef __cplusplus
extern "C" {
#endif

struct backend;

struct backend *backend_create(struct wl_display *display);

void backend_destroy(struct backend *self);

bool backend_connect(struct backend *self);

#ifdef __cplusplus
}
#endif

#endif  //  OPENVR_WITH_WAYLAND_BACKEND_H
