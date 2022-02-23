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
    offset/=4;
    len/=4;
    uint32_t x = offset / (_screen.width);
    uint32_t y = offset % (_screen.width);
    uint32_t h = (offset + len) / (_screen.width) - x;
    uint32_t w = (offset + len) % (_screen.width) - y;
    //printf("offet: %d %d %d %d %d\n",offset,x,y,x+h,y+w);
    _draw_rect(buf,offset,len/4,w,h,1);
}

void init_device() {
  memcpy(dispinfo,"WIDTH:400\nHEIGHT:300\n",23);

  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
