#include <driver.h>
#include <machine/mem.h>

/* Interrupt enable register */
#define IER           (UART0 + 0x01)
#define IER_RX_ENABLE (0b1 << 0)
#define IER_TX_ENABLE (0b1 << 1)

/* Line control register */
#define LCR           (UART0 + 0x03)
#define LCR_8BITSWORD (0b11 << 0)
#define LCR_SETBAUD   (0b1 << 7)

/* FIFO control register */
#define FCR        (UART0 + 2)
#define FCR_CLEAR  (0b11 << 1)
#define FCR_ENABLE (0b1 << 0)

static void init(void);

struct driver
driver_uart(void)
{
	struct driver d = {
		.init = init,
	};

	return d;
};

void
init(void)
{
	/* Temporarily disable interrupts */
	*IER = 0;

	/* Switch to set-baud mode */
	*LCR = LCR_SETBAUD;

	/* LSB for baud rate of 38.4K */
	*(UART0 + 0x0) = 0x03;

	/* MSB for baud rate of 38.4K */
	*(UART0 + 0x1) = 0x00;

	/* Switch to normal mode and set word length to 8 bits */
	*LCR = LCR_8BITSWORD;

	/* Reset and enable FIFOs */
	*FCR = FCR_ENABLE | FCR_CLEAR;

	/* Enable transmit and receive interrupts */
	*IER = IER_RX_ENABLE | IER_TX_ENABLE;
}
