#include <am.h>
#include <x86.h>

#define RTC_PORT 0x48   // Note that this is not standard
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
  unsigned long now=inl(RTC_PORT);
  long msseconds = now - boot_time;
  return msseconds;
}
uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};

size_t get_screen_size(){
  return _screen.height*_screen.width;
}

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int offset, int len) {
  memcpy(fb + offset, pixels, 4 * len);
  return;
}

void _draw_sync() {
}

int _read_key() {
  int key=_KEY_NONE;;
  uint8_t status=0;
  status=inb(0x64);
  if(status) key=inl(0x60);
  return key;
}
