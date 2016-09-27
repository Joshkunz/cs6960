#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "interval.h"

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

static int unsigned_to_signed(unsigned int v) {
    assert((v & MASK) == v && "value must be 6 bit only");
    if (v > interval::MAX) {
        return interval::MIN + (interval::MIN + ((int) v));
    } else {
        return (int) v;
    }
}

std::set<int> * concretize(interval &iv) {
    std::set<int> * s = new std::set<int>();
    for (int i = iv.lo; i <= iv.hi; i++) {
        s->insert(i);
    }

    return s;
}

std::set<int> * transfer_sub(std::set<int> &s_lhs, std::set<int> &s_rhs) {
    std::set<int> * s = new std::set<int>();
    for (int lhs : s_lhs) {
        for (int rhs : s_rhs) {
            int sub_v;
            sub(lhs, rhs, &sub_v);
            s->insert(sub_v);
        }
    }
    return s;
}

std::set<int> * transfer_and(std::set<int> &s_lhs, std::set<int> &s_rhs) {
    std::set<int> * s = new std::set<int>();
    for (int lhs : s_lhs) {
        for (int rhs : s_rhs) {
            int result;
            unsigned int u_lhs, u_rhs, u_result;
            memcpy(&u_lhs, &lhs, sizeof(int));
            memcpy(&u_rhs, &rhs, sizeof(int));
            u_lhs &= MASK;
            u_rhs &= MASK;
            u_result = u_lhs & u_rhs;
            result = unsigned_to_signed(u_result);
            s->insert(result);
        }
    }
    return s;
}

interval lift_abstract(std::set<int> & s) {
    int min = *std::min_element(s.cbegin(), s.cend());
    int max = *std::max_element(s.cbegin(), s.cend());

    //for (int i = min; i <= max; i++) {
    //    /* if an element is not found */
    //    if (s.find(i) == s.end()) {
    //        return top;
    //    }
    //}

    return interval(min, max);
}

std::vector<interval> * all_intervals(void) {
    std::vector<interval> * v = new std::vector<interval>();
    for (int i = interval::MIN; i <= interval::MAX; i++) {
        for (int j = i; j <= interval::MAX; j++) {
            v->push_back(interval(i, j));
        }
    }
    return v;
}

void test_all_sub(bool quieter) {
    printf("Testing all sub:\n");
    auto intervals = all_intervals();
    for (interval i : *intervals) {
        for (interval j : *intervals) {
            if (! quieter) { printf("."); }
            auto i_concrete = concretize(i);
            auto j_concrete = concretize(j);
            auto result = transfer_sub(*i_concrete, *j_concrete);
            interval abstract_result = lift_abstract(*result);
            interval test_result = i - j;
            if (! (test_result == abstract_result)) {
                std::cerr << "\n"
                          << "Testing " << i << " - " << j << " failed:\n"
                          << "    Got = " << test_result << "\n"
                          << "    Expected = " << abstract_result << "\n";
                exit(1);
            }
            delete i_concrete; 
            delete j_concrete;
            delete result;
        }
    }
    delete intervals;
    printf("\nPASSED\n");
}

void test_all_and(bool quieter) {
    printf("Testing all and:\n");
    auto intervals = all_intervals();
    bool non_trivial = false;
    for (interval i : *intervals) {
        for (interval j : *intervals) {
            if (! quieter) { printf("."); }
            auto i_concrete = concretize(i);
            auto j_concrete = concretize(j);
            auto result = transfer_and(*i_concrete, *j_concrete);
            interval abstract_result = lift_abstract(*result);
            interval test_result = i & j;
            if (! (test_result == interval::top() )) { non_trivial = true; }
            /* Make sure the test result is at least a generalization of
             * the actual answer. */
            if (! (test_result >= abstract_result)) {
                std::cerr << "\n"
                          << "Testing " << i << " - " << j << " failed:\n"
                          << "    Got = " << test_result << "\n"
                          << "    Expected = " << abstract_result << "\n";
                exit(1);
            }
            delete i_concrete; 
            delete j_concrete;
            delete result;
        }
    }
    delete intervals;
    if (non_trivial != true) {
        printf("\nWARNING: Always returns top\n");
    }
    printf("\nPASSED\n");
}

int main(int argc, char * argv[]) {
    /* Hardcode to quiet for submission */
    bool quieter = true;
    if (argc > 1 && std::string(argv[1]) == std::string("-q")) {
        quieter = true;
    }

    test_all_sub(quieter);
    test_all_and(quieter);
    return 0;
}
