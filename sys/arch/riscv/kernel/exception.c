#include "exception.h"

#include <arch.h>
#include <serial.h>

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
		serial_print("Exception raised: "
		             "Instruction address misaligned.\n");
		break;
	case CAUSE_INSTR_ACC_FAULT:
		serial_print("Exception raised: Instruction access fault.\n");
		break;
	case CAUSE_ILLEGAL_INSTR:
		serial_print("Exception raised: Illegal instruction.\n");
		break;
	case CAUSE_BREAKPOINT:
		serial_print("Exception raised: Breakpoint.\n");
		break;
	case CAUSE_LOAD_ADDR_MISAL:
		serial_print("Exception raised: Load address misaligned.\n");
		break;
	case CAUSE_LOAD_ACC_FAULT:
		serial_print("Exception raised: Load access fault.\n");
		break;
	case CAUSE_STORE_ADDR_MISAL:
		serial_print("Exception raised: "
		             "Store/AMO address misaligned.\n");
		break;
	case CAUSE_STORE_ACC_FAULT:
		serial_print("Exception raised: Store/AMO access fault.\n");
		break;
	case CAUSE_UMODE_ECALL:
		serial_print("Exception raised: "
		             "Environment call from U-mode.\n");
		break;
	case CAUSE_SMODE_ECALL:
		serial_print("Exception raised: "
		             "Environment call from S-mode.\n");
		break;
	case CAUSE_MMODE_ECALL:
		serial_print("Exception raised: "
		             "Environment call from M-mode.\n");
		break;
	case CAUSE_INSTR_PAGE_FAULT:
		serial_print("Exception raised: Instruction page fault.\n");
		break;
	case CAUSE_LOAD_PAGE_FAULT:
		serial_print("Exception raised: Load page fault.\n");
		break;
	case CAUSE_STORE_PAGE_FAULT:
		serial_print("Exception raised: Store/AMO page fault.\n");
		break;
	case CAUSE_DOUBLE_TRAP:
		serial_print("Exception raised: Double trap.\n");
		break;
	case CAUSE_SOFTWARE_CHECK:
		serial_print("Exception raised: Software check.\n");
		break;
	case CAUSE_HARDWARE_ERROR:
		serial_print("Exception raised: Hardware error.\n");
		break;
	default:
		serial_print("Exception raised: Unkown.\n");
	}
}
