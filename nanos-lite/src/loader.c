#include "common.h"
#include "ramdisk.h"
#include "fs.h"
#define DEFAULT_ENTRY ((void *)0xc000000)
extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
uintptr_t loader(_Protect *as, const char *filename) {
  // TODO();
  printf("%s\n",filename);
  int fd=fs_open(filename,0,0);
  fs_read(fd,DEFAULT_ENTRY,RAMDISK_SIZE);
  return (uintptr_t)DEFAULT_ENTRY;
}
