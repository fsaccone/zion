#include <log.h>

#include <arch/serial.h>

void
panic(const char *m)
{
	serial_print(m);
	serial_print("\n");
	for (;;);
}
