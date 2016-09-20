#include "five_bit.h"
#include "interval.h"

#include <string>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <climits>

using namespace std;

static const interval top = interval(MIN_5BIT, MAX_5BIT);

static void assert_valid_range(const int lo, const int hi) {
    assert_is_5bit(lo);
    assert_is_5bit(hi);
    assert(lo <= hi && "low value must be less than high value");
}

void interval::assert_valid_interval() {
    assert_valid_range(this->lo, this->hi);
}

std::string interval::string(void) {
    return "(" + std::to_string(this->lo)
               + ", "
               + std::to_string(this->hi)
               + ")";
}

interval::interval(int low, int high) {
    assert_valid_range(low, high);
    this->lo = low;
    this->hi = high;
}

bool interval::is_subset(const interval & i) {
    return (this->lo <= i.low()) && (this->hi >= i.high());
}

interval interval::subtract(interval a, interval b) {
    a.assert_valid_interval();
    b.assert_valid_interval();

    int positive_result, negative_result;

    /* This could possibly overflow positive */
    bool positive_overflow = sub_5bit(a.hi, b.lo, &positive_result);

    /* This could possibly overflow negative */
    bool negative_overflow = sub_5bit(a.lo, b.hi, &negative_result);

    /* If neither direction overflows, then just return the appropriate interval */
    if (positive_overflow == negative_overflow && (negative_result <= positive_result)) {
        return interval(negative_result, positive_result);
    } else {
        return top;
    }
}

static unsigned int interval_members_or(interval &iv) {
    unsigned int result = 0x0;
    unsigned int val;
    for (int i = iv.low(); i <= iv.high(); i++) {
        memcpy(&val, &i, sizeof(unsigned int));
        result |= val & MASK_5BIT;
    }
    return result;
}

interval interval::bitwise_and(interval a, interval b) {
    a.assert_valid_interval();
    b.assert_valid_interval();

    /* Figure out which bits are set to 0 in all cases. */
    unsigned int a_or = interval_members_or(a);
    unsigned int b_or = interval_members_or(b);

    /* Then and those values together to find the bits that will be zero
     * in all results, no matter which pair of integers is chosen. If a bit is
     * set to zero in this value, then it will be zero in all executions. */
    unsigned int guaranteed = a_or & b_or;

    /* now we just need to find the lowest and highest possible
     * numbers assuming that we can only change ones from guaranteed to zeros */
    int lower_bound;
    int upper_bound;

    /* If the result can possibly have the sign bit set (i.e. could be negative) */
    if (guaranteed & 0x10) {
        /* We know that the most significant bit might be a one, which means
         * that the remaining bits (if they are set to 1) will only increase the
         * value. Therefore, we can assume the worst case of -16. */
        lower_bound = -16;
        /* The most positive is just assuming all the 1s are set, except for
         * the highest. */
        memcpy(&upper_bound, &guaranteed, sizeof(int));
        upper_bound &= 0x0f;
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
