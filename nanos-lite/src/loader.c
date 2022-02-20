#include "common.h"
#include "ramdisk.h"
#define DEFAULT_ENTRY ((void *)0x4000000)

uintptr_t loader(_Protect *as, const char *filename) {
  // TODO();
  ramdisk_read(DEFAULT_ENTRY,0,as->area.start-as->area.end);
  return (uintptr_t)DEFAULT_ENTRY;
}
