#include "common.h"
#include "fs.h"
enum {
  SYS_none,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_exit,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};
int sys_nano(){
  return 1;
}

void sys_exit(int status){
  _halt(status);
}

int sys_write(int fd,const char* start,uint32_t len){
  Log("%s %d\n",start,len);
  if(fd==0||fd==1||fd==2){
    for(uint32_t i=0;i<len;i++){
      _putc(*(start+i));
    }
  } else if(fd>6){
    fs_write(fd,(void*)start,len);
  }
  return len;
}
extern void* _end;
int sys_brk(uintptr_t inc){
  _end=(void*)inc;
  return 0;
}

int sys_read(int fd, void *buf, size_t count){
  return fs_read(fd,(void*)buf,count);
}

int sys_lseek(int fd, off_t offset, int whence){
  return fs_lseek(fd,offset,whence);
}

int sys_open(const char *path, int flags, uint32_t mode){
 return fs_open(path,flags,mode);
}

int sys_close(int fd){
  fs_close(fd);
  return 0;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);
  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_nano(); break;
    case SYS_write: SYSCALL_ARG1(r)=sys_write(a[1],(const char*)(a[2]),a[3]); break;
    case SYS_exit: sys_exit(SYSCALL_ARG2(r)); break;
    case SYS_brk: SYSCALL_ARG1(r)=sys_brk(a[1]); break;
    case SYS_read: SYSCALL_ARG1(r)=sys_read(a[1],(void*)a[2],a[3]); break;
    case SYS_lseek: SYSCALL_ARG1(r)=sys_lseek(a[1],a[2],a[3]); break;
    case SYS_close: SYSCALL_ARG1(r)=sys_close(a[1]); break;
    case SYS_open: SYSCALL_ARG1(r)=sys_open((const char*)a[1],a[2],a[3]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
