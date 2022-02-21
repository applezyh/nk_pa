#include "common.h"
#include "syscall.h"

int sys_nano(){
  return 1;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);

  switch (a[0]) {
    case SYS_none: SYSCALL_ARG1(r)=sys_nano(); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
