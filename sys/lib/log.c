#include <log.h>

#include <serial.h>

void
debug(const char *m)
{
	serial_print("DEBUG: ");
	serial_print(m);
	serial_print("\n");
}

void
panic(const char *m)
{
	serial_print("PANIC: ");
	serial_print(m);
	serial_print("\n");
	for (;;);
}
