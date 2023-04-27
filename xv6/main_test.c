// #include "types.h"
// #include "defs.h"
// #include "param.h"
// #include "memlayout.h"
// #include "mmu.h"
// #include "proc.h"
// #include "x86.h"


// __attribute__((__aligned__(PGSIZE)))
// pde_t pgtable1[NPDENTRIES];
// __attribute__((__aligned__(PGSIZE)))
// pde_t pgtable2[NPDENTRIES];

// __attribute__((__aligned__(PGSIZE)))
// pde_t entrypgdir[NPDENTRIES];



// void init_page(){
//     uint addr = 0;
//     int i = 0;
//     for(i = 0; i < NPDENTRIES; i++){
//         pgtable1[i] = addr << 12 |  PTE_P | PTE_W | PTE_PS;
//         pgtable2[i] = addr << 12 |  PTE_P | PTE_W | PTE_PS;
//         addr += PGSIZE;
//     }

//     entrypgdir[0] = ((unsigned int)pgtable1) | PTE_P | PTE_W | PTE_PS;
//     entrypgdir[KERNBASE>>PDXSHIFT] = ((unsigned int)pgtable2) | PTE_P | PTE_W | PTE_PS;

    
//     asm volatile ("jmp _start");
// }