#ifndef __FIVE_BIT_H__
#define __FIVE_BIT_H__

const extern int MIN_5BIT;
const extern int MAX_5BIT;
const extern unsigned int MASK_5BIT;

void assert_is_5bit(const int i);

/* Perform a 5 bit subtraction of b from a with wrapping
 * semantics. The result is stored in the memory location 
 * pointed to by `result`. If the subtraction overflows, 'true'
 * is returned, otherwise 'false' is returned. */
bool sub_5bit(int a, int b, int *result);

/* Convert an unsigned integer (with a 5 bit value into a signed integer) */
int unsigned_to_signed_5bit(unsigned int v);

#endif

