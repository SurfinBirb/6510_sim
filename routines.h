/* #ifndef _adr 
#define _adr uint8_t data, uint8_t *mem_map, rf regs, uint8_t pc_incr
#endif */
#define CARRY 0
#define ZERO 1
#define IRQ 2
#define DECIMAL 3
#define BREAK 4
#define OVERFLOW 6
#define NEGATIVE 7

#include "routines.c"

void routine(bool debug, es exec_s);
void fetch(es *exec_s);
void decode(es *exec_s);
void execute(es *exec_s);
