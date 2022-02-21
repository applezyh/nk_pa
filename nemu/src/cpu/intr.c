#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  //TODO();
  rtl_push(&cpu.eflag);
  rtl_push(&ret_addr);
  rtl_push(&cpu.cs);
  vaddr_t entry=vaddr_read(cpu.IDTentry+NO,4);
  printf("%x\n",entry);
  cpu.eip=entry;
}

void dev_raise_intr() {
}
