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
    uint32_t x = offset / _screen.height;
    uint32_t y = offset % _screen.height;
    uint32_t h = (offset + len) / _screen.height - x;
    uint32_t w = (offset + len) % _screen.height;
    printf("%d %d %d %d %d %d %d %d\n",offset,len,x,y,h,w,_screen.width,_screen.height);
    _draw_rect(buf,x,y,w,h);
}

void init_device() {
  memcpy(dispinfo,"WIDTH:400\nHEIGHT:300\n",23);

  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
