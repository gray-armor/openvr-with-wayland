#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-server.h>

#include "internal.h"

struct server {
  struct wl_display *display;
  struct mode_switcher *mode_switcher;
  struct backend *backend;

  struct wl_listener switch_mode_listener;
};

static int
on_term_signal(int signal_number, void *data)
{
  struct server *server = data;

  fprintf(stderr, "Caught signal: %s\n", strsignal(signal_number));

  wl_display_terminate(server->display);

  return 1;
}

static void
on_switch_mode(struct wl_listener *listener, void *data)
{
  struct server *server =
      wl_container_of(listener, server, switch_mode_listener);
  uint32_t mode = *(uint32_t *)data;
  (void)server;
  fprintf(stderr, "Switching mode to %d\n", mode);

  backend_connect(server->backend);

  fprintf(stderr, "Connected to OpenVR runtime\n");
}

int
main()
{
  struct server server;
  struct wl_event_loop *loop;
  struct wl_event_source *signals[3];
  int ret = EXIT_FAILURE;

  server.display = wl_display_create();
  if (server.display == NULL) {
    fprintf(stderr, "Failed to create a wl_display\n");
    goto err;
  }

  if (wl_display_add_socket(server.display, "openvr-wayland-0") != 0) {
    fprintf(stderr, "Failed to add socket\n");
    goto err_display;
  }

  loop = wl_display_get_event_loop(server.display);
  signals[0] = wl_event_loop_add_signal(loop, SIGTERM, on_term_signal, &server);
  signals[1] = wl_event_loop_add_signal(loop, SIGINT, on_term_signal, &server);
  signals[2] = wl_event_loop_add_signal(loop, SIGQUIT, on_term_signal, &server);

  if (!signals[0] || !signals[1] || !signals[2]) goto err_display;

  server.backend = backend_create(server.display);
  if (server.backend == NULL) {
    fprintf(stderr, "Failed to create a backend");
    goto err_signals;
  }

  server.mode_switcher = mode_switcher_create(server.display);
  if (server.mode_switcher == NULL) {
    fprintf(stderr, "Failed to create a mode switcher\n");
    goto err_backend;
  }

  server.switch_mode_listener.notify = on_switch_mode;
  wl_signal_add(
      &server.mode_switcher->switch_mode_signal, &server.switch_mode_listener);

  wl_display_run(server.display);

  ret = EXIT_SUCCESS;

  wl_list_remove(&server.switch_mode_listener.link);

  mode_switcher_destroy(server.mode_switcher);

err_backend:
  backend_destroy(server.backend);

err_signals:
  for (int i = ARRAY_LENGTH(signals) - 1; i >= 0; i--)
    if (signals[i]) wl_event_source_remove(signals[i]);

err_display:
  wl_display_destroy(server.display);

err:
  return ret;
}
