#include "syscall.h"
int sys_nano(){
  return 1;
}

void sys_exit(int status){
  _halt(status);
}

int sys_write(int fd,uint8_t* start,uint32_t len){
  for(uint8_t* i=start;i<start+len;i++){
    _putc(*i);
  }
  return 0;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_nano(); break;
    case SYS_exit: sys_exit(SYSCALL_ARG2(r)); break;
    case SYS_write: sys_write(SYSCALL_ARG2(r),(uint8_t*)SYSCALL_ARG3(r),SYSCALL_ARG4(r)); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
