#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

#include "asm.h"




static void mpmain(void)  __attribute__((noreturn));
extern pde_t *kpgdir;
extern char end[]; // first address after kernel loaded from ELF file

void entry(void);

int main(void);

__attribute__((__aligned__(PGSIZE)))
pde_t pgtable1[NPDENTRIES];
__attribute__((__aligned__(PGSIZE)))
pde_t pgtable2[NPDENTRIES];

__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES];


void init_page(){

    pde_t *pgtb1 = (pde_t*)V2P(pgtable1);
    pde_t *pgtb2 = (pde_t*)V2P(pgtable2);
    pde_t *pgtb = (pde_t*)V2P(entrypgdir);

    // uint addr = 0;
    int i = 0;
    for(i = 0; i < NPDENTRIES; i++){
        pgtb1[i] = i << 12 |  PTE_P | PTE_W;
        pgtb2[i] = i << 12 |  PTE_P | PTE_W;

        // addr += PGSIZE;
    }

    pgtb[0] = ((unsigned int)pgtb1) | PTE_P | PTE_W;
    pgtb[KERNBASE>>PDXSHIFT] = ((unsigned int)pgtb2) | PTE_P | PTE_W;


    asm volatile("jmp %0" : : "r"(V2P_WO(entry)));

    // // movl    $(V2P_WO(entrypgdir)), %eax
    // asm volatile("movl %0, %%eax"::"i"(V2P_WO(entrypgdir)));
    // // movl    %eax, %cr3
    // asm volatile("movl %%eax, %%cr3");
    // // # Turn on paging.
    // // movl    %cr0, %eax
    // asm volatile("orl %0, %%eax"::"i"((CR0_PG|CR0_WP)));
    // // orl     $(CR0_PG|CR0_WP), %eax
    // asm volatile("movl %%eax, %%cr0");
    // // movl    %eax, %cr0

    // // # Set up the stack pointer.
    // // movl $(stack + KSTACKSIZE), %esp
    // asm volatile("movl %0, %%esp"::"i"(stack + KSTACKSIZE));
    
    // // asm volatile ("jmp _start");
    // asm volatile("addl %0, %%esp" : : "i"(KERNBASE));
    // /* Jump to init_cond() to continue initialization. */
    // asm volatile("jmp *%0" : : "r"(main));
    // // init_cond();
    // panic("should not reach here");
}


// Bootstrap processor starts running C code here.
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
int
main(void)
{
  kinit1(end, P2V(4*1024*1024)); // phys page allocator
  kvmalloc();      // kernel page table
  // mpinit();        // detect other processors
  // lapicinit();     // interrupt controller
  seginit();       // segment descriptors
  pic_init();       // disable pic
  // ioapicinit();    // another interrupt controller
  consoleinit();   // console hardware
  uartinit();      // serial port
  pinit();         // process table
  tvinit();        // trap vectors
  binit();         // buffer cache
  fileinit();      // file table
  ideinit();       // disk 
  // startothers();   // start other processors
  kinit2(P2V(4*1024*1024), P2V(PHYSTOP)); // must come after startothers()
  userinit();      // first user process
  mpmain();        // finish this processor's setup
}

// Other CPUs jump here from entryother.S.
// static void
// mpenter(void)
// {
//   switchkvm();
//   seginit();
//   lapicinit();
//   mpmain();
// }

// Common CPU setup code.
static void
mpmain(void)
{
  cprintf("cpu%d: starting %d\n", cpuid(), cpuid());
  idtinit();       // load idt register
  xchg(&(mycpu()->started), 1); // tell startothers() we're up
  scheduler();     // start running processes
}

// pde_t entrypgdir[];  // For entry.S

// Start the non-boot (AP) processors.
// static void
// startothers(void)
// {
//   extern uchar _binary_entryother_start[], _binary_entryother_size[];
//   uchar *code;
//   struct cpu *c;
//   char *stack;

//   // Write entry code to unused memory at 0x7000.
//   // The linker has placed the image of entryother.S in
//   // _binary_entryother_start.
//   code = P2V(0x7000);
//   memmove(code, _binary_entryother_start, (uint)_binary_entryother_size);

//   for(c = cpus; c < cpus+ncpu; c++){
//     if(c == mycpu())  // We've started already.
//       continue;

//     // Tell entryother.S what stack to use, where to enter, and what
//     // pgdir to use. We cannot use kpgdir yet, because the AP processor
//     // is running in low  memory, so we use entrypgdir for the APs too.
//     stack = kalloc();
//     *(void**)(code-4) = stack + KSTACKSIZE;
//     *(void(**)(void))(code-8) = mpenter;
//     *(int**)(code-12) = (void *) V2P(entrypgdir);

//     lapicstartap(c->apicid, V2P(code));

//     // wait for cpu to finish mpmain()
//     while(c->started == 0)
//       ;
//   }
// }

// The boot page table used in entry.S and entryother.S.
// Page directories (and page tables) must start on page boundaries,
// hence the __aligned__ attribute.
// PTE_PS in a page directory entry enables 4Mbyte pages.

// __attribute__((__aligned__(PGSIZE)))
// pde_t entrypgdir[NPDENTRIES] = {
//   // Map VA's [0, 4MB) to PA's [0, 4MB)
//   [0] = (0) | PTE_P | PTE_W | PTE_PS,
//   // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
//   [KERNBASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
// };





//PAGEBREAK!
// Blank page.
//PAGEBREAK!
// Blank page.
//PAGEBREAK!
// Blank page.

