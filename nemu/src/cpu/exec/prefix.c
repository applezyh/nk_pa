#include "cpu/exec.h"

make_EHelper(real);

make_EHelper(operand_size) {
  decoding.is_operand_size_16 = true;
  memset(decoding.asm_buf,0,128);
  instr_fetch(eip, 1);
  exec_real(eip);
  decoding.is_operand_size_16 = false;
}
