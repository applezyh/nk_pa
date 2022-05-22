#include "common.h"
extern _RegSet* schedule(_RegSet *prev);
extern _RegSet* do_syscall(_RegSet *prev);
#define _EVENT_SYSCALL 8
extern _RegSet* do_syscall(_RegSet *r);
static _RegSet* do_event(_Event e, _RegSet* r) {
  switch (e.event) {
    case _EVENT_SYSCALL: return do_syscall(r); break;
    case _EVENT_TRAP: printf("event:self-trapped\n");return schedule(r); break; 
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  _asye_init(do_event);
}
