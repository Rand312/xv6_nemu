#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

make_helper_v(push_i)
make_helper_v(push_r)
make_helper_v(push_rm)

make_helper(push_ds){
    uint32_t val = cpu.ds.selector;
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val);

    print_asm_template1();
    return 1;
}

make_helper(push_es){
    uint32_t val = cpu.es.selector;
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val);

    print_asm_template1();
    return 1;
}

make_helper(push_fs){
    uint32_t val = cpu.fs.selector;
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val);

    print_asm_template1();
    return 1;
}

make_helper(push_gs){
    uint32_t val = cpu.gs.selector;
    reg_l(R_ESP) -= 4;
    swaddr_write(reg_l(R_ESP), 4, val);

    print_asm_template1();
    return 1;
}