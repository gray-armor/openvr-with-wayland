#include <stdio.h>

#include "internal.h"
#include "zen-desktop-protocol.h"

static void
mode_switcher_handle_destroy(struct wl_resource *resource)
{
  wl_list_remove(&resource->link);
}

static void
mode_switcher_protocol_switch_mode(
    struct wl_client *client, struct wl_resource *resource, uint32_t mode)
{
  struct mode_switcher *self = wl_resource_get_user_data(resource);
  (void)client;
  (void)resource;

  wl_signal_emit(&self->switch_mode_signal, &mode);
}

static const struct zen_desktop_mode_switcher_interface interface = {
    .switch_mode = mode_switcher_protocol_switch_mode,
};

static void
mode_switcher_bind(
    struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct mode_switcher *self = data;
  struct wl_resource *resource;

  resource = wl_resource_create(
      client, &zen_desktop_mode_switcher_interface, version, id);
  if (resource == NULL) {
    fprintf(stderr, "Failed to create a wl_resource\n");
    wl_client_post_no_memory(client);
    return;
  }

  wl_resource_set_implementation(
      resource, &interface, self, mode_switcher_handle_destroy);

  wl_list_insert(&self->resources, &resource->link);
}

struct mode_switcher *
mode_switcher_create(struct wl_display *display)
{
  struct mode_switcher *self;

  self = zalloc(sizeof *self);
  if (self == NULL) {
    fprintf(stderr, "No memory\n");
    goto err;
  }

  self->display = display;
  wl_list_init(&self->resources);
  wl_signal_init(&self->switch_mode_signal);
  self->global = wl_global_create(display, &zen_desktop_mode_switcher_interface,
      1, self, mode_switcher_bind);

  return self;

err:
  return NULL;
}

void
mode_switcher_destroy(struct mode_switcher *self)
{
  wl_list_remove(&self->resources);
  wl_global_destroy(self->global);
  free(self);
}
