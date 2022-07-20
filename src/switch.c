#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <wayland-client-core.h>

#include "zen-desktop-client-protocol.h"

struct app {
  struct wl_display *display;
  struct wl_registry *registry;
  struct zen_desktop_mode_switcher *switcher;
};

static void
mode_switcher_protocol_switched(void *data,
    struct zen_desktop_mode_switcher *zen_desktop_mode_switcher, uint32_t mode)
{
  struct app *app = data;
  (void)app;
  (void)zen_desktop_mode_switcher;
  fprintf(stderr, "switched to %d\n", mode);
}

static const struct zen_desktop_mode_switcher_listener mode_switcher_listener =
    {
        .switched = mode_switcher_protocol_switched,
};

static void
registry_global(void *data, struct wl_registry *wl_registry, uint32_t name,
    const char *interface, uint32_t version)
{
  struct app *app = data;
  if (strcmp(interface, "zen_desktop_mode_switcher") == 0) {
    app->switcher = wl_registry_bind(
        wl_registry, name, &zen_desktop_mode_switcher_interface, version);
    zen_desktop_mode_switcher_add_listener(
        app->switcher, &mode_switcher_listener, app);
  }
}

static void
registry_global_remove(
    void *data, struct wl_registry *wl_registry, uint32_t name)
{
  (void)data;
  (void)wl_registry;
  (void)name;
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

int
main()
{
  struct app app;
  fd_set fds;
  int fd;

  app.display = wl_display_connect("openvr-wayland-0");
  assert(app.display);

  app.registry = wl_display_get_registry(app.display);
  assert(app.registry);

  wl_registry_add_listener(app.registry, &registry_listener, &app);

  wl_display_roundtrip(app.display);
  assert(app.switcher);

  zen_desktop_mode_switcher_switch_mode(
      app.switcher, ZEN_DESKTOP_MODE_SWITCHER_DESKTOP_MODE_3D);

  fd = wl_display_get_fd(app.display);

  wl_display_flush(app.display);

  while (1) {
    int ret;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    ret = select(fd + 1, &fds, NULL, NULL, NULL);

    if (ret < 0 && errno == EINTR) continue;
    if (ret < 0) goto err;
    if (ret == 0) continue;

    if (!FD_ISSET(fd, &fds)) continue;

    while (wl_display_prepare_read(app.display) != 0) {
      if (errno != EAGAIN) goto err;
      wl_display_dispatch_pending(app.display);
    }

    wl_display_flush(app.display);

    wl_display_read_events(app.display);
    wl_display_dispatch_pending(app.display);
  }

  return EXIT_SUCCESS;

err:
  return EXIT_FAILURE;
}
