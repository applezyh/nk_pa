#include "common.h"
#include "ramdisk.h"
#define DEFAULT_ENTRY ((void *)0xc000000)
extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
uintptr_t loader(_Protect *as, const char *filename) {
  // TODO();
  ramdisk_read(DEFAULT_ENTRY,0,RAMDISK_SIZE);
  return (uintptr_t)DEFAULT_ENTRY;
}
