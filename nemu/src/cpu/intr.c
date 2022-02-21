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
  uint32_t gate[2];

  printf("%x\n",cpu.IDTentry);

  gate[0] = vaddr_read(cpu.IDTentry+NO*sizeof(GateDesc),4);
  gate[1] = vaddr_read(cpu.IDTentry+NO*sizeof(GateDesc)+4,4);

  GateDesc g = *((GateDesc*)gate);
  cpu.eip= g.offset_15_0+(g.offset_31_16<<16);

  printf("%x\n",cpu.eip);
  TODO();
}

void dev_raise_intr() {
}
