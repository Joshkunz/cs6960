#include "util.h"
#include "interval.h"

#include <string>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <climits>

using namespace std;

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

static unsigned int interval_members_or(const interval &iv) {
    unsigned int result = 0x0;
    unsigned int val;
    for (int i = iv.lo; i <= iv.hi; i++) {
        memcpy(&val, &i, sizeof(unsigned int));
        result |= val & UTIL_MASK;
    }
    return result;
}

interval interval::operator&(const interval &rhs) {
    /* TODO: Check if there is a better way to get an interval from a pointer. */
    interval &lhs = *this;

    /* Figure out which bits are set to 0 in all cases. */
    unsigned int lhs_or = interval_members_or(lhs);
    unsigned int rhs_or = interval_members_or(rhs);

    /* Then and those values together to find the bits that will be zero
     * in all results, no matter which pair of integers is chosen. If a bit is
     * set to zero in this value, then it will be zero in all executions. */
    unsigned int guaranteed = lhs_or & rhs_or;

    /* now we just need to find the lowest and highest possible
     * numbers assuming that we can only change ones from guaranteed to zeros */
    int lower_bound;
    int upper_bound;

    /* If the result can possibly have the sign bit set (i.e. could be negative) */
    if (guaranteed & 0x20) {
        /* We know that the most significant bit might be a one, which means
         * that the remaining bits (if they are set to 1) will only increase the
         * value. Therefore, we can assume the worst case of -16. */
        lower_bound = interval::MIN;
        /* The most positive is just assuming all the 1s are set, except for
         * the highest. */
        memcpy(&upper_bound, &guaranteed, sizeof(int));
        upper_bound &= 0x1f;
    /* otherwise we know the result *cannot* be negative. */
    } else {
        /* So the lower bound is 0, which is the case when all ones are set to zero. */
        lower_bound = 0;
        /* And the upper bound is just when all possible 1s are set to 1. */
        memcpy(&upper_bound, &guaranteed, sizeof(int));
    }

    /* So, our final interval is just (lower_bound, upper_bound) */
    return interval(lower_bound, upper_bound);
}
