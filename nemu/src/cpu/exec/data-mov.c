#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
  printf("push\n");
  switch (decoding.opcode)
  {
  case 0x50:
    rtl_push(&(cpu.eax));
    print_asm("pushw %s","eax");
    break;

  case 0x51:
    rtl_push(&(cpu.ecx));
    print_asm("pushw %s","ecx");
    break;

  case 0x52:
    rtl_push(&(cpu.edx));
    print_asm("pushw %s","edx");
    break;

  case 0x53:
    rtl_push(&(cpu.ebx));
    print_asm("pushw %s","ebx");
    break;

  case 0x54:
    rtl_push(&(cpu.esp));
    print_asm("pushw %s","esp");
    break;

  case 0x55:
    rtl_push(&(cpu.ebp));
    print_asm("pushw %s","ebp");
    break;

  case 0x56:
    rtl_push(&(cpu.esi));
    print_asm("pushw %s","esi");
    break;

  case 0x57:
    rtl_push(&(cpu.edi));
    print_asm("pushw %s","edi");
    break;
  
  case 0xff:
    rtl_push(&(id_src->val));
    print_asm_template1(push);
    break;

  default:
    break;
  }
}

make_EHelper(pop) { 
  TODO();

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
