#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "five_bit.h"
#include "interval.h"

const interval top = interval(-16, 15);

std::set<int> * concretize(interval &iv) {
    std::set<int> * s = new std::set<int>();
    for (int i = iv.low(); i <= iv.high(); i++) {
        s->insert(i);
    }

    return s;
}

std::set<int> * transfer_sub(std::set<int> &s_lhs, std::set<int> &s_rhs) {
    std::set<int> * s = new std::set<int>();
    for (int lhs : s_lhs) {
        for (int rhs : s_rhs) {
            int sub;
            sub_5bit(lhs, rhs, &sub);
            assert(sub >= -16 && sub <= 15 && "produced sub value not in valid range");
            s->insert(sub);
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
            u_lhs &= 0x1f;
            u_rhs &= 0x1f;
            u_result = u_lhs & u_rhs;
            result = unsigned_to_signed_5bit(u_result);
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
    for (int i = -16; i <= 15; i++) {
        for (int j = i; j <= 15; j++) {
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
            interval test_result = interval::subtract(i, j);
            if (! (test_result == abstract_result)) {
                fprintf(stderr, "\n");
                fprintf(stderr, "Testing %s - %s failed:\n", i.string().c_str(),
                                                             j.string().c_str());
                fprintf(stderr, "    Got = %s\n", test_result.string().c_str());
                fprintf(stderr, "    Expected = %s\n", abstract_result.string().c_str());
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
            interval test_result = interval::bitwise_and(i, j);
            if (! (test_result == top)) { non_trivial = true; }
            /* Make sure the test result is at least a generalization of
             * the actual answer. */
            if (! test_result.is_subset(abstract_result)) {
                fprintf(stderr, "\n");
                fprintf(stderr, "Testing %s - %s failed:\n", i.string().c_str(),
                                                             j.string().c_str());
                fprintf(stderr, "    Got = %s\n", test_result.string().c_str());
                fprintf(stderr, "    Expected subset of %s\n", abstract_result.string().c_str());
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
    bool quieter = false;
    if (argc > 1 && std::string(argv[1]) == std::string("-q")) {
        quieter = true;
    }

    test_all_sub(quieter);
    test_all_and(quieter);
    return 0;
}
