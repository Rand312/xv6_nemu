#include "cpu/exec/template-start.h"

#define instr pushf

// static void pushf() {
//     current_sreg = R_SS;
//     uint32_t val = cpu.EFLAGS;
//     reg_l(R_ESP) -= 4;
//     swaddr_write(reg_l(R_ESP), 4, val);

//     print_asm_template1();
// }

make_helper(pushf){
    current_sreg = R_SS;
    uint32_t val = cpu.EFLAGS;
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val);

    print_asm_template1();

    return 1;
}

#include "cpu/exec/template-end.h"