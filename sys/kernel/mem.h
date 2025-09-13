#ifndef __MEM_H
#define __MEM_H

/* Frees all machine memory regions mapped as usable. Returns 0 normally and -1
   in case of error. */
int freeallmem(void);

/* Disables virtual memory to use physical instead. */
void physicalmem(void);

#endif
