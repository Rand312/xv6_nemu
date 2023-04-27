#include "cpu/exec/template-start.h"

#define instr rol

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t count = src & 0x1f;
    uint8_t temp = count;

    uint8_t tmpcf = 0;
    while(temp != 0){
        tmpcf = (dest >> (sizeof(DATA_TYPE) - 1)) & 1;
        dest = dest * 2 + tmpcf;
        temp--;
    }
    OPERAND_W(op_dest, dest);
    if (count == 1){
        if ((dest >> (sizeof(DATA_TYPE) - 1)) & 1 != cpu.CF){
            cpu.OF = 1;
        }else{
            cpu.OF = 0;
        }
    }
	// dest >>= count;
	// OPERAND_W(op_dest, dest);

	// DATA_TYPE ret = dest;
	// cpu.CF = 0;
	// cpu.OF = 0;
	// cpu.ZF = !ret;
    // cpu.SF = ret >> ((DATA_BYTE << 3) - 1);
	// ret ^= ret >> 4;
    // ret ^= ret >> 2;
    // ret ^= ret >> 1;
    // ret &= 1;
    // cpu.PF = !ret;
	// /* There is no need to update EFLAGS, since no other instructions 
	//  * in PA will test the flags updated by this instruction.
	//  */

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
