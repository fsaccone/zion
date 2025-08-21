#include "exception.h"

#include <arch/types.h>

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
	(void)cause;
}
