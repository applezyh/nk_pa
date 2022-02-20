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

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  uint32_t seq=0;
  for(int i=y;i<y+h;i++){
    for(int j=x;j<x+w;j++){
      fb[i*_screen.width+j]=pixels[seq++];
    }
  }
}

void _draw_sync() {
}

int _read_key() {
  int key=_KEY_NONE;
  uint8_t status=0;
  while(!status){
    status=inb(0x64);
  }
  key = inl(0x60);
  return key;
}
