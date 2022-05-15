#include "common.h"
#include "ramdisk.h"

/* The kernel is monolithic, therefore we do not need to
 * translate the address `buf' from the user process to
 * a physical one, which is necessary for a microkernel.
 */

/* read `len' bytes starting from `offset' of ramdisk into `buf' */
void ramdisk_read(void *buf, off_t offset, size_t len) {
  //assert(offset + len <= RAMDISK_SIZE);
  memcpy(buf, &ramdisk_start + offset, offset+len<=RAMDISK_SIZE?len:RAMDISK_SIZE-offset);
}

/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
void ramdisk_write(const void *buf, off_t offset, size_t len) {
  //assert(offset + len <= RAMDISK_SIZE);
  memcpy(&ramdisk_start + offset, buf, offset+len<=RAMDISK_SIZE?len:RAMDISK_SIZE-offset);
}

void init_ramdisk() {
  Log("ramdisk info: start = %p, end = %p, size = %d bytes",
      &ramdisk_start, &ramdisk_end, RAMDISK_SIZE);
}

size_t get_ramdisk_size() {
  return RAMDISK_SIZE;
}
