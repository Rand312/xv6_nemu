#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr outs


	// uint8_t imm8 = instr_fetch(cpu.eip+1,1);
	// pio_write(imm8,DATA_BYTE,REG(R_EAX));
	// print_asm("out\t%x",imm8);
	// return 2;

make_helper(concat(outs_, SUFFIX)) {
    current_sreg = R_ES;
    DATA_TYPE tmp = MEM_R(reg_l(R_EDI));
    uint16_t port = reg_l(R_EDX);
    pio_write(port, DATA_BYTE, tmp);
	if (cpu.DF == 0) reg_l(R_EDI) += DATA_BYTE;
	else reg_l(R_EDI) -= DATA_BYTE;
	print_asm("outs");
    return 1;
}


#include "cpu/exec/template-end.h"