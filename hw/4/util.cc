#include "interval.h"
#include <cassert> 

const extern unsigned int UTIL_MASK = 0x3f;

void assert_is_valid_value(const int i) {
    assert((i >= interval::MIN && i <= interval::MAX) && "value must be in valid range");
}

bool sub(int a, int b, int *result) {
    assert_is_valid_value(a);
    assert_is_valid_value(b);

    bool did_overflow = false;

    int res = a - b;
    if (res < interval::MIN) {
        did_overflow = true;
        *result = res & UTIL_MASK;
    }
    else if (res > interval::MAX) {
        did_overflow = true;
        *result = interval::MIN + (interval::MIN + res);
    } else {
        *result = res;
    }
    return did_overflow;
}

int unsigned_to_signed(unsigned int v) {
    assert((v & UTIL_MASK) == v && "value must be 6 bit only");
    if (v > interval::MAX) {
        return interval::MIN + (interval::MIN + ((int) v));
    } else {
        return (int) v;
    }
}
