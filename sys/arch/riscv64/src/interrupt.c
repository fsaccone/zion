#include "interrupt.h"

#include <arch.h>
#include <log.h>

#define CAUSE_SSI 1
#define CAUSE_MSI 3
#define CAUSE_STI 5
#define CAUSE_MTI 7
#define CAUSE_SEI 9
#define CAUSE_MEI 11
#define CAUSE_COI 13

void
handleinterrupt(uintn cause)
{
	switch (cause) {
	case CAUSE_SSI:
	case CAUSE_MSI:
		panic("Interrupt raised: Software interrupt.");
		break;
	case CAUSE_STI:
	case CAUSE_MTI:
		panic("Interrupt raised: Timer interrupt.");
		break;
	case CAUSE_SEI:
	case CAUSE_MEI:
		panic("Interrupt raised: External interrupt.");
		break;
	case CAUSE_COI:
		panic("Interrupt raised: Counter overflow interrupt.");
		break;
	default:
		panic("Interrupt raised: Unknown.");
	}
}
