#include "cpu/exec/template-start.h"
#include <nemu.h>
// #include "reg.h"

#define instr iret
#if DATA_BYTE == 2
int popfromstack_w(){
    DATA_TYPE ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif

#if DATA_BYTE == 4
int popfromstack_l(){
    DATA_TYPE ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif

make_helper(concat(iret_,SUFFIX)) {
	if (cpu.cr0.protect_enable == 0){
		cpu.eip = concat(popfromstack_,SUFFIX)();
		cpu.cs.selector = concat(popfromstack_,SUFFIX)();
		if(cpu.eip == 0){
			panic("stop\n");
		}
		cpu.EFLAGS = concat(popfromstack_,SUFFIX)();
	}else{
		current_sreg = R_SS;
		cpu.eip = concat(popfromstack_,SUFFIX)();

		uint16_t newcs = concat(popfromstack_,SUFFIX)();
		uint16_t oldcs = cpu.cs.selector;
		cpu.cs.selector = newcs;
		cpu.EFLAGS = concat(popfromstack_,SUFFIX)();
		
		char new_rpl = newcs & 3;
		char old_rpl = oldcs & 3;   //cpl???
		if(old_rpl != new_rpl){
			uint32_t user_esp = concat(popfromstack_,SUFFIX)();
			uint16_t user_ss_selector = concat(popfromstack_,SUFFIX)();

			uint32_t bases = cpu.tr.base;
			uint32_t limits = cpu.tr.limit;

			uint32_t tss_addr = bases;
			
			swaddr_write(tss_addr + 8, 4, cpu.ss.selector);
			swaddr_write(tss_addr + 4, 4, cpu.esp);
			// ts->esp0 = cpu.esp;


			cpu.esp = user_esp;
			cpu.ss.selector = user_ss_selector;
		}
		
		current_sreg = R_CS;
		sreg_load(R_CS);
		sreg_load(R_SS);
	}
	print_asm("iret");
	return 0;
}

#include "cpu/exec/template-end.h"