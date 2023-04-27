#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"// CR0、CR3

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS};
/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct{
	uint16_t selector;// visible
	/*invisible*/
	uint16_t attribute;//read,write,execute
	uint32_t limit;
	uint32_t base;
}Segment_Reg;

typedef struct {
	uint16_t selector;
	uint32_t base;
	uint32_t limit;
}TaskReg;

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];
		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
			union{
				struct{
					uint32_t CF:	1;
					uint32_t :		1;
					uint32_t PF:	1;
					uint32_t :		1;
					uint32_t AF:	1;
					uint32_t :		1;
					uint32_t ZF:	1;
					uint32_t SF:	1;
					uint32_t TF:	1;
					uint32_t IF:	1;
					uint32_t DF:	1;
					uint32_t OF:	1;
					uint32_t IOPL:	2;
					uint32_t NT:	1;
					uint32_t :		1;
					uint32_t RF:	1;
					uint32_t VM:	1;
					uint32_t :		14;
				};
				uint32_t EFLAGS;
			};
		};
	};

	/* Do NOT change the order of the GPRs' definitions. */
	
//	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	swaddr_t eip;

	struct GDTR{
		uint32_t base;
		uint16_t limit;
	}gdtr;

	CR0 cr0;

	union{
		struct{
			Segment_Reg sreg[6];
		};
		struct {
			Segment_Reg es, cs, ss, ds, fs, gs;
		};
	};

	TaskReg tr;

	CR3 cr3;

	struct IDTR{
		uint32_t base;
		uint16_t limit;
	}idtr;

	bool INTR;
} CPU_state;


typedef struct{
	union{
		struct{
			uint16_t limit1;
			uint16_t base1;
		};
		uint32_t part1;
	};
	union{
		struct{
			uint32_t base2:		8;
			uint32_t a:			1;
			uint32_t type:		3;
			uint32_t s:			1;
			uint32_t dpl:		2;
			uint32_t p:			1;
			uint32_t limit2:	4;
			uint32_t avl:		1;
			uint32_t :			1;
			uint32_t x:			1;
			uint32_t g:			1;
			uint32_t base3:		8;
		};
		uint32_t part2;
	};
}Sreg_Descriptor;

typedef struct {
	union {
		struct {
			uint32_t p 	:1;
			uint32_t rw	:1;
			uint32_t us	:1;
			uint32_t 	:2;
			uint32_t a	:1;
			uint32_t d 	:1;
			uint32_t 	:2;
			uint32_t avail	:3;
			uint32_t addr 	:20;
		};
		uint32_t val;
	};
}Page_Descriptor;

typedef struct {
	union {
		struct {
			uint32_t offset1 :16;
			uint32_t selector :16;
		};
		uint32_t part1;
	};
	union{
		struct{ 
			uint32_t pad0 :8;
			uint32_t type :4;
			uint32_t system :1;
			uint32_t dpl :2;
			uint32_t present :1;
			uint32_t offset2 :16;
		};
		uint32_t part2;
	};
}Gate_Descriptor;

Sreg_Descriptor *sreg_desc;
Gate_Descriptor *idt_desc;


// Task state segment format
struct taskstate {
  uint32_t link;         // Old ts selector
  uint32_t esp0;         // Stack pointers and segment selectors
  uint16_t ss0;        //   after an increase in privilege level
  uint16_t padding1;
  uint32_t *esp1;
  uint16_t ss1;
  uint16_t padding2;
  uint32_t *esp2;
  uint16_t ss2;
  uint16_t padding3;
  void *cr3;         // Page directory base
  uint32_t *eip;         // Saved state from last task switch
  uint32_t eflags;
  uint32_t eax;          // More saved state (registers)
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t *esp;
  uint32_t *ebp;
  uint32_t esi;
  uint32_t edi;
  uint16_t es;         // Even more saved state (segment selectors)
  uint16_t padding4;
  uint16_t cs;
  uint16_t padding5;
  uint16_t ss;
  uint16_t padding6;
  uint16_t ds;
  uint16_t padding7;
  uint16_t fs;
  uint16_t padding8;
  uint16_t gs;
  uint16_t padding9;
  uint16_t ldt;
  uint16_t padding10;
  uint16_t t;          // Trap on task switch
  uint16_t iomb;       // I/O map base address
};


extern CPU_state cpu;
uint8_t current_sreg;
void sreg_load(uint8_t);
void tr_load();

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char *sregs[];

#endif
