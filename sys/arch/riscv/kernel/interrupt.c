#include "interrupt.h"

#include <arch.h>
#include <serial.h>

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
	switch (cause) {
	case CAUSE_SSI:
	case CAUSE_MSI:
		serial_print("Interrupt raised: Software interrupt.\n");
		break;
	case CAUSE_STI:
	case CAUSE_MTI:
		serial_print("Interrupt raised: Timer interrupt.\n");
		break;
	case CAUSE_SEI:
	case CAUSE_MEI:
		serial_print("Interrupt raised: External interrupt.\n");
		break;
	case CAUSE_COI:
		serial_print("Interrupt raised: "
		             "Counter overflow interrupt.\n");
		break;
	default:
		serial_print("Interrupt raised: Unkown.\n");
	}
}
