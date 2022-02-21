#include "common.h"
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

int sys_write(uint8_t* start,uint32_t len, int fd){
  for(uint8_t* i=start;i<start+len;i++){
    _putc(*i);
  }
  return 0;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  printf("call %d %d %d %d\n",a[0],SYSCALL_ARG2(r),SYSCALL_ARG3(r),SYSCALL_ARG4(r));
  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_nano(); break;
    case SYS_write: sys_write((uint8_t*)SYSCALL_ARG2(r),SYSCALL_ARG3(r),SYSCALL_ARG4(r)); break;
    case SYS_exit: sys_exit(SYSCALL_ARG2(r)); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
