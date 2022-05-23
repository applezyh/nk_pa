#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC];
static int nr_proc = 0;
PCB *current = NULL;

uintptr_t loader(_Protect *as, const char *filename);

void load_prog(const char *filename) {
  int i = nr_proc ++;
  _protect(&pcb[i].as);

  uintptr_t entry = loader(&pcb[i].as, filename);

  // TODO: remove the following three lines after you have implemented _umake()
  // _switch(&pcb[i].as);
  //current = &pcb[i];
  //Log("%x", entry);
  //((void (*)(void))entry)();
  _Area stack;
  stack.start = pcb[i].stack;
  stack.end = stack.start + sizeof(pcb[i].stack);
  pcb[i].tf = _umake(&pcb[i].as, stack, stack, (void *)entry, NULL, NULL);
  Log("pcb[%d] esp: %x", i, pcb[i].tf->esp);
}

_RegSet* schedule(_RegSet *prev) {
  current=&pcb[0];
  if(current != NULL)
    current->tf = prev;
  Log("ptr=0x%x", (uint32_t)current->as.ptr);
  _switch(&current->as);
  Log("schedule return esp %x", current->tf->esp);
  return current->tf;
}
