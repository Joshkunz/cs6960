from collections import namedtuple

# the number of bits our transfer function is defined over
NUM_BITS = 4

Interval = namedtuple("Interval", ["low", "high"])

# four bit unsigned subtraction transfer function
def unsigned_sub(lhs : Interval, rhs : Interval) -> Interval:
    subs = [lhs.low - rhs.low, lhs.low - rhs.high,
            lhs.high - rhs.low, lhs.high - rhs.high]
    subs = [x % (2 ** NUM_BITS) for x in subs]
    return Interval(min(subs), max(subs))

if __name__ == "__main__":
    assert unsigned_sub(Interval(6, 7), Interval(2, 3)) == Interval(3, 5)
    assert unsigned_sub(Interval(2, 7), Interval(1, 7)) == Interval(0, 11)
