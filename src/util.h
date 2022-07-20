#ifndef OPENVR_WITH_WAYLAND_UTIL_H
#define OPENVR_WITH_WAYLAND_UTIL_H

#include <stdlib.h>

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a)[0])
#endif

static inline void*
zalloc(size_t size)
{
  return calloc(1, size);
}

#endif  //  OPENVR_WITH_WAYLAND_UTIL_H
