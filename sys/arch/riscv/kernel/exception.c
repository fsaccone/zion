#include "exception.h"

#include <arch.h>
#include <log.h>

#define CAUSE_INSTR_ADDR_MISAL 0
#define CAUSE_INSTR_ACC_FAULT  1
#define CAUSE_ILLEGAL_INSTR    2
#define CAUSE_BREAKPOINT       3
#define CAUSE_LOAD_ADDR_MISAL  4
#define CAUSE_LOAD_ACC_FAULT   5
#define CAUSE_STORE_ADDR_MISAL 6
#define CAUSE_STORE_ACC_FAULT  7
#define CAUSE_UMODE_ECALL      8
#define CAUSE_SMODE_ECALL      9
#define CAUSE_MMODE_ECALL      11
#define CAUSE_INSTR_PAGE_FAULT 12
#define CAUSE_LOAD_PAGE_FAULT  13
#define CAUSE_STORE_PAGE_FAULT 15
#define CAUSE_DOUBLE_TRAP      16
#define CAUSE_SOFTWARE_CHECK   18
#define CAUSE_HARDWARE_ERROR   19

void
handle_exception(uint32_t cause)
{
	switch (cause) {
	case CAUSE_INSTR_ADDR_MISAL:
		panic("Exception raised: Instruction address misaligned.");
		break;
	case CAUSE_INSTR_ACC_FAULT:
		panic("Exception raised: Instruction access fault.");
		break;
	case CAUSE_ILLEGAL_INSTR:
		panic("Exception raised: Illegal instruction.");
		break;
	case CAUSE_BREAKPOINT:
		panic("Exception raised: Breakpoint.");
		break;
	case CAUSE_LOAD_ADDR_MISAL:
		panic("Exception raised: Load address misaligned.");
		break;
	case CAUSE_LOAD_ACC_FAULT:
		panic("Exception raised: Load access fault.");
		break;
	case CAUSE_STORE_ADDR_MISAL:
		panic("Exception raised: Store/AMO address misaligned.");
		break;
	case CAUSE_STORE_ACC_FAULT:
		panic("Exception raised: Store/AMO access fault.");
		break;
	case CAUSE_UMODE_ECALL:
		panic("Exception raised: Environment call from U-mode.");
		break;
	case CAUSE_SMODE_ECALL:
		panic("Exception raised: Environment call from S-mode.");
		break;
	case CAUSE_MMODE_ECALL:
		panic("Exception raised: Environment call from M-mode.");
		break;
	case CAUSE_INSTR_PAGE_FAULT:
		panic("Exception raised: Instruction page fault.");
		break;
	case CAUSE_LOAD_PAGE_FAULT:
		panic("Exception raised: Load page fault.");
		break;
	case CAUSE_STORE_PAGE_FAULT:
		panic("Exception raised: Store/AMO page fault.");
		break;
	case CAUSE_DOUBLE_TRAP:
		panic("Exception raised: Double trap.");
		break;
	case CAUSE_SOFTWARE_CHECK:
		panic("Exception raised: Software check.");
		break;
	case CAUSE_HARDWARE_ERROR:
		panic("Exception raised: Hardware error.");
		break;
	default:
		panic("Exception raised: Unkown.");
	}
}
