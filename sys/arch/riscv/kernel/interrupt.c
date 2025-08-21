#include "interrupt.h"

#include <arch/types.h>

#define CAUSE_SSI 1
#define CAUSE_MSI 3
#define CAUSE_STI 5
#define CAUSE_MTI 7
#define CAUSE_SEI 9
#define CAUSE_MEI 11
#define CAUSE_COI 13

void
handle_interrupt(uint32_t cause)
{
	(void)cause;
}
