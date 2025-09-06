#ifndef __MEM_H
#define __MEM_H

/* Frees all machine memory regions mapped as usable */
void freeallmem(void);

/* Disables virtual memory to use physical instead */
void physicalmem(void);

#endif
