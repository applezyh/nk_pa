#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  memcpy(buf,dispinfo+offset,len);
}

void fb_write(const void *buf, off_t offset, size_t len) {
    int x = offset / _screen.height;
    int y = offset % _screen.height;
    int h = (offset + len) / _screen.height - x;
    int w = (offset + len) % _screen.height - y;
    _draw_rect(buf,x,y,h,w);
}

void init_device() {
  memcpy(dispinfo,"WIDTH:400 HEIGHT:300",28);
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
