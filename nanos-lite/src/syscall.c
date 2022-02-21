#include "common.h"
#include "syscall.h"

int sys_nano(){
  return 1;
}

void sys_exit(int status){
  _halt(status);
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);

  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_nano(); break;
    case SYS_exit: sys_exit(a[0]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
