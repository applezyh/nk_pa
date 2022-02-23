#include "fs.h"
#include "ramdisk.h"
typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0, 0},
#include "files.h"
};
size_t fs_filesz(int fd){
  return file_table[fd].size;
}
#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode){
  for(size_t i=0;i<NR_FILES;i++){
    if(!strcmp(pathname,file_table[i].name)){
      return i;
    }
  }
  return -1;
}
ssize_t fs_read(int fd, void *buf, size_t len){
  off_t disk_off = file_table[fd].disk_offset;
  off_t open_off = file_table[fd].open_offset;
  ramdisk_read(buf,disk_off+open_off,len);
  file_table[fd].open_offset+=len;
  return len;
}
ssize_t fs_write(int fd, const void *buf, size_t len){
  
  off_t disk_off = file_table[fd].disk_offset;
  off_t open_off = file_table[fd].open_offset;
  assert(open_off+len<file_table[fd].size);
  ramdisk_write(buf,disk_off+open_off,len);
  file_table[fd].open_offset+=len;
  return len;
}
off_t fs_lseek(int fd, off_t offset, int whence){
  switch (whence)
  {
  case SEEK_SET: file_table[fd].open_offset=offset;break;
  case SEEK_CUR: file_table[fd].open_offset+=offset;break;
  case SEEK_END: file_table[fd].open_offset=(file_table[fd].size+file_table[fd].disk_offset+offset);break;
    /* code */
  default:
    assert(0);
  }
  return file_table[fd].open_offset+file_table[fd].disk_offset;
}
int fs_close(int fd){
  file_table[fd].open_offset=0;
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
