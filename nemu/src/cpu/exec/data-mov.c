#include "cpu/exec.h"

make_EHelper(mov) {
  if(cpu.eip==0x4000308){
    printf("%d\n",decoding.is_operand_size_16);
  }
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
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

  case 0x54:{
    uint32_t temp = cpu.esp;
    rtl_push(&(temp));
    print_asm("pushw %s","esp");
    break;
  }

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
    rtl_push(&(id_dest->val));
    print_asm_template1(push);
    break;

  case 0x6a:
    rtl_push(&(id_dest->val));
    print_asm_template1(push);
    break;

  case 0x06:
    rtl_push(&(cpu.es));
    print_asm_template1(push);
    break;

  default:
    rtl_push(&(id_dest->val));
    print_asm_template1(push);
    break;
  }
}

make_EHelper(pop) { 
  switch (decoding.opcode)
  {
  case 0x58:
    rtl_pop(&(cpu.eax));
    print_asm("pop %s","eax");
    break;

  case 0x59:
    rtl_pop(&(cpu.ecx));
    print_asm("pop %s","ecx");
    break;

  case 0x5a:
    rtl_pop(&(cpu.edx));
    print_asm("pop %s","edx");
    break;

  case 0x5b:
    rtl_pop(&(cpu.ebx));
    print_asm("pop %s","ebx");
    break;

  case 0x5c:
    rtl_pop(&(cpu.esp));
    print_asm("pushw %s","esp");
    break;

  case 0x5d:
    rtl_pop(&(cpu.ebp));
    print_asm("pop %s","ebp");
    break;

  case 0x5e:
    rtl_pop(&(cpu.esi));
    print_asm("pop %s","esi");
    break;

  case 0x5f:
    rtl_pop(&(cpu.edi));
    print_asm("pop %s","edi");
    break;

  default:
    TODO();
    break;
  }

}

make_EHelper(pusha) {
  //TODO();
  if(decoding.is_operand_size_16){
    uint32_t sp,ax,cx,dx,bx,bp,si,di;
    rtl_lr_l(&ax,0);
    rtl_lr_l(&cx,1);
    rtl_lr_l(&dx,2);
    rtl_lr_l(&bx,3);
    rtl_lr_l(&sp,4);
    rtl_lr_l(&bp,5);
    rtl_lr_l(&si,6);
    rtl_lr_l(&di,7);
    rtl_push(&ax);
    rtl_push(&cx);
    rtl_push(&dx);
    rtl_push(&bx);
    rtl_push(&sp);
    rtl_push(&bp);
    rtl_push(&si);
    rtl_push(&di);
  } else {
    uint32_t temp = cpu.esp;
    rtl_push(&(cpu.eax));
    rtl_push(&(cpu.ecx));
    rtl_push(&(cpu.edx));
    rtl_push(&(cpu.ebx));
    rtl_push(&(temp));
    rtl_push(&(cpu.ebp));
    rtl_push(&(cpu.esi));
    rtl_push(&(cpu.edi));
  }

  print_asm("pusha");
}

make_EHelper(popa) {
  //TODO();
    uint32_t temp;
    rtl_pop(&(cpu.edi));
    rtl_pop(&(cpu.esi));
    rtl_pop(&(cpu.ebp));
    rtl_pop(&(temp));
    rtl_pop(&(cpu.ebx));
    rtl_pop(&(cpu.edx));
    rtl_pop(&(cpu.ecx));
    rtl_pop(&(cpu.eax));
    cpu.esp=temp;

  print_asm("popa");
}

make_EHelper(leave) {
  // TODO();
  rtl_mv(&(cpu.esp),&(cpu.ebp));
  rtl_pop(&(cpu.ebp));
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    // TODO();
  }
  else {
    // TODO();
    uint32_t flag;
    rtl_lr_l(&flag, R_AX);
    flag>>=15;
    uint32_t re1=0xffff;
    uint32_t re2=0x0000;
    if(flag) rtl_sr_l(R_DX,&re1);
    else rtl_sr_l(R_DX,&re2);
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
