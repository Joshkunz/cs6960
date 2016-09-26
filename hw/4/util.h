#ifndef __UTIL_H__
#define __UTIL_H__

extern unsigned int UTIL_MASK;

void assert_is_valid_value(const int i);

/* Perform a 6 bit subtraction of b from a with wrapping
 * semantics. The result is stored in the memory location 
 * pointed to by `result`. If the subtraction overflows, 'true'
 * is returned, otherwise 'false' is returned. */
bool sub(int a, int b, int *result);

/* Convert an unsigned integer (with a 6 bit value into a signed integer) */
int unsigned_to_signed(unsigned int v);

#endif

