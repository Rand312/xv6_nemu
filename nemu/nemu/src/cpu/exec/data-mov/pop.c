#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

make_helper_v(pop_rm)
make_helper_v(pop_r)

make_helper(pop_ds){
    uint32_t val = swaddr_read(reg_l(R_ESP), 4);
    cpu.sreg[R_DS].selector = (uint16_t)val;
    sreg_load(R_DS);
    reg_l(R_ESP) += 4;
   
    print_asm_template1();
    return 1;

}

make_helper(pop_es){
    uint32_t val = swaddr_read(reg_l(R_ESP), 4);
    cpu.sreg[R_ES].selector = (uint16_t)val;
    sreg_load(R_ES);
    reg_l(R_ESP) += 4;
   
    print_asm_template1();
    return 1;

}

make_helper(pop_gs){
    uint32_t val = swaddr_read(reg_l(R_ESP), 4);
    cpu.sreg[R_GS].selector = (uint16_t)val;
    // sreg_load(R_GS);
    reg_l(R_ESP) += 4;
   
    print_asm_template1();
    return 1;

}
make_helper(pop_fs){
    uint32_t val = swaddr_read(reg_l(R_ESP), 4);
    cpu.sreg[R_FS].selector = (uint16_t)val;
    // sreg_load(R_FS);
    reg_l(R_ESP) += 4;
    print_asm_template1();
    return 1;

}