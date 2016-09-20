#include <cassert> 

const extern int MIN_5BIT = -16;
const extern int MAX_5BIT = 15;
const extern unsigned int MASK_5BIT = 0x1f;

void assert_is_5bit(const int i) {
    assert((i >= MIN_5BIT && i <= MAX_5BIT) 
            && "value must be between -16 and 15 (inclusive)");
}

bool sub_5bit(int a, int b, int *result) {
    assert_is_5bit(a);
    assert_is_5bit(b);

    bool did_overflow = false;

    int res = a - b;
    if (res < MIN_5BIT) {
        did_overflow = true;
        *result = res & MASK_5BIT;
    }
    else if (res > MAX_5BIT) {
        did_overflow = true;
        *result = MIN_5BIT + (MIN_5BIT + res);
    } else {
        *result = res;
    }
    return did_overflow;
}

int unsigned_to_signed_5bit(unsigned int v) {
    assert((v & MASK_5BIT) == v && "value must be 5 bit only");
    if (v > MAX_5BIT) {
        return MIN_5BIT + (MIN_5BIT + ((int) v));
    } else {
        return (int) v;
    }
}
