#include "cpu/exec.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  decoding.is_jmp = 1;
  print_asm("jmp %x", decoding.jmp_eip);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  if((subcode&0xe)==14){
    printf("%x       %d %d flag : %d  target : %x",cpu.eip,cpu.ecx,cpu.edx,t2,decoding.jmp_eip);
        for(int i=0;i<20;i++){
          int num=vaddr_read(0x100180+i*4,4);
          printf("%d ",num);
    }
    printf("\n\n");
  }
  decoding.is_jmp = t2;


  print_asm("j%s %x", get_cc_name(subcode), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  decoding.jmp_eip = id_dest->val;
  decoding.is_jmp = 1;

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  decoding.jmp_eip = id_dest->val+*eip;
  rtl_push(eip);
  decoding.is_jmp = 1;
  // TODO();
  print_asm("call %x", decoding.jmp_eip);
}

make_EHelper(ret) {
  // TODO();
  uint32_t ret_addr;
  rtl_pop(&ret_addr);
  decoding.jmp_eip = ret_addr;
  decoding.is_jmp = 1;
  print_asm("ret");
}

make_EHelper(call_rm) {
  TODO();

  print_asm("call *%s", id_dest->str);
}
