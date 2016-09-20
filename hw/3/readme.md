# Homework 3

`interval.cc` contains my actual definition of the interval transfer functions
for subtraction and bitwise AND over 5 bit signed integers. `test.cc` contains
a test driver that does exhaustive testing of both of the transfer functions.
Naïve transfer functions that concretize, perform the transfer, and then abstract
the concrete results are used as test oracles.

## Running the tests

You can run the tests using:

```bash
make
./test -q
```

(the `-q` option just silences a lot of debugging information.)
