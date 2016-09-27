#include "interval.h"
#include <cassert> 

static const unsigned int MASK = 0x3f;

static bool sub(int a, int b, int *result) {

    bool did_overflow = false;

    int res = a - b;
    if (res < interval::MIN) {
        did_overflow = true;
        *result = res & MASK;
    }
    else if (res > interval::MAX) {
        did_overflow = true;
        *result = interval::MIN + (interval::MIN + res);
    } else {
        *result = res;
    }
    return did_overflow;
}

interval interval::operator-(const interval &rhs) {

    /* TODO: Check if there is a better way to get an interval from a pointer. */
    interval &lhs = *this;

    int positive_result, negative_result;

    /* This could possibly overflow positive */
    bool positive_overflow = sub(lhs.hi, rhs.lo, &positive_result);

    /* This could possibly overflow negative */
    bool negative_overflow = sub(lhs.lo, rhs.hi, &negative_result);

    /* If neither direction overflows, then just return the appropriate interval */
    if (positive_overflow == negative_overflow && (negative_result <= positive_result)) {
        return interval(negative_result, positive_result);
    } else {
        return interval::top();
    }
}
