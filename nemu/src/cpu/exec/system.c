#include "cpu/exec.h"

void diff_test_skip_qemu();
void diff_test_skip_nemu();



make_EHelper(lidt) {
  // TODO();
  uint16_t data0,data1,data2;
  data0 = vaddr_read(id_dest->addr+0,2);
  data1 = vaddr_read(id_dest->addr+2,2);
  data2 = vaddr_read(id_dest->addr+4,2);
  rtlreg_t idt_entry=data1+(data2<<16); 
  uint16_t idt_len=data0;
  cpu.IDTentry=idt_entry;
  cpu.IDTlen=idt_len;

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
  //TODO();
  Log("int ret addr: 0x%x", decoding.seq_eip);
  *eip=raise_intr(id_dest->val, decoding.seq_eip);
  
  print_asm("int %s", id_dest->str);
#ifdef DIFF_TEST
  diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
  //TODO();
  rtl_pop(&cpu.cs);
  rtl_pop(eip);
  rtl_pop(&cpu.eflag);
  Log("iret to: 0x%x", *eip);
  print_asm("iret");
}

uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
  // TODO();
  uint32_t data = pio_read(id_src->val,id_dest->width);
  operand_write(id_dest,&data);

  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  // TODO();
  
  pio_write(id_dest->val,id_src->width,id_src->val);
  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
