#include "cpu/exec/helper.h"

make_helper(ltr){
    cpu.tr.selector = op_src->val;
    tr_load();

    print_asm_template1();

    return 2;
}