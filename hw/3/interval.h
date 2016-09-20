#include <string>

#ifndef __INTERVAL_H__
#define __INTERVAL_H__

class interval {
  // 5-bit signed intervals: valid values for lo and hi are -16..15 and it is
  // required that hi >= lo. the bounds are inclusive.
  int lo, hi;

public:
  interval(int lo, int hi);

  inline bool operator== (const interval &rhs) {
      return (this->lo == rhs.lo) && (this->hi == rhs.hi);
  };

  std::string string(void);

  /* bombs the program if the interval is not valid */
  void assert_valid_interval(void);

  bool is_subset(const interval &i);

  // these functions must trigger an assertion violation when presented with any
  // invalid interval, and must never return an invalid interval

  // requirement: be sound and fully precise
  // the second argument is to be subtracted from the first
  static interval subtract(interval, interval);

  // requirement: be sound and don't always return top
  static interval bitwise_and(interval, interval);

  int low(void) const { return this->lo; }
  int high(void) const { return this->hi; }
};

#endif 
