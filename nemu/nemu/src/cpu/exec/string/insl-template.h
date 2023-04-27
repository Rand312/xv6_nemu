#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr ins


// static void do_execute () {
// 	OPERAND_W(op_dest,pio_read(op_src -> addr,DATA_BYTE));
// 	print_asm_template2();
// }

// make_helper(concat(ins_,SUFFIX)){
// 	uint8_t imm8 = instr_fetch(cpu.eip+1,1);
// 	REG(R_EAX) = pio_read(imm8,DATA_BYTE);
// 	print_asm("in\t%x",imm8);
// 	return 2;
// }
// make_helper(concat(in_r2rm_,SUFFIX)){
// 	REG(R_EAX) = pio_read((ioaddr_t)reg_w(R_EDX),DATA_BYTE);
// 	print_asm("in");
// 	return 1;
// }
// // make_instr_helper(si2a);
// // make_instr_helper(r2rm);

// #include "cpu/exec/template-end.h"


make_helper(concat(ins_, SUFFIX)) {
    current_sreg = R_ES;
    // DATA_TYPE tmp = MEM_R(reg_l(R_EDX));
    uint16_t port = reg_l(R_EDX);
    DATA_TYPE tmp = pio_read(port, DATA_BYTE);
    MEM_W(reg_l(R_EDI),tmp);
	if (cpu.DF == 0) reg_l(R_EDI) += DATA_BYTE;
	else reg_l(R_EDI) -= DATA_BYTE;
	print_asm("insl");
    return 1;
}


#include "cpu/exec/template-end.h"