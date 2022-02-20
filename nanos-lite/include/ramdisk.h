/* The kernel is monolithic, therefore we do not need to
 * translate the address `buf' from the user process to
 * a physical one, which is necessary for a microkernel.
 */

/* read `len' bytes starting from `offset' of ramdisk into `buf' */
void ramdisk_read(void *buf, off_t offset, size_t len);

/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
void ramdisk_write(const void *buf, off_t offset, size_t len);

void init_ramdisk();

size_t get_ramdisk_size();