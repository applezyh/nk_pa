#include "common.h"
#include "ramdisk.h"
#include "fs.h"
#include "memory.h"
#define DEFAULT_ENTRY ((void *)0x8048000)
extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
uintptr_t loader(_Protect *as, const char *filename) {
	int fd = fs_open(filename, 0, 0);
	int filesize = fs_filesz(fd);
	void *vaddr, *page;
	vaddr = DEFAULT_ENTRY;

	while (filesize > 0) {
		page = new_page();
		_map(as, vaddr, page);
		fs_read(fd, page, PGSIZE);
		vaddr += PGSIZE;
		filesize -= PGSIZE;
	}
	fs_close(fd);

	return (uintptr_t)DEFAULT_ENTRY;
}
