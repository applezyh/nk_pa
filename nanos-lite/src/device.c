#include "common.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  unsigned long time =  _uptime();
  char tbuf[100];
  size_t n = sprintf(tbuf,"t %u\n",time);
  memcpy(buf,tbuf,n>len?len:n);
  return n-1>len?len:n-1;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  memcpy(buf,dispinfo+offset,len);
}


void fb_write(const void *buf, off_t offset, size_t len) {
    offset/=4;
    len/=4;
    _draw_rect(buf,offset,len,0,0,1);
}

void init_device() {
  memcpy(dispinfo,"WIDTH:400\nHEIGHT:300\n",23);

  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
