#include "value.hpp"
#include <getopt.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

Klass CBigNum, CFloat;

template <typename T>
__attribute__((noinline))
void test_fixnum(int nloop,
                 int vecsize)
{
    T *in0 = alloc_value_vec<T>(vecsize);
    T *in1 = alloc_value_vec<T>(vecsize);
    T sum;

    for (int i=0; i<vecsize; i++) {
        in0[i] = T::from_int(0);
        in1[i] = T::from_int(1);
    }

    sum = T::from_int(0);

    for (int li=0; li<nloop; li++) {
        for (int vi=0; vi<vecsize; vi++) {
            sum = sum + (in0[vi] + in1[vi]);
        }
    }

    std::cout << sum.to_int() << '\n';
}

template <typename T>
__attribute__((noinline))
void test_float(int nloop,
                int vecsize)
{
    T *in0 = alloc_value_vec<T>(vecsize);
    T *in1 = alloc_value_vec<T>(vecsize);
    T sum;

    for (int i=0; i<vecsize; i++) {
        in0[i] = T::from_float(0);
        in1[i] = T::from_float(1);
    }

    sum = T::from_float(0);

    for (int li=0; li<nloop; li++) {
        for (int vi=0; vi<vecsize; vi++) {
            sum = sum + (in0[vi] + in1[vi]);
        }
    }

    std::cout << sum.to_float() << '\n';
}

int main(int argc, char **argv)
{
    int nloop = 16384;
    int vecsize = 4096;
    enum type t = TYPE_FIXNUM;
    int opt;
    bool is_included = false;

    while ((opt=getopt(argc, argv, "n:s:t:T:")) != -1) {
        switch (opt) {
        case 'n':
            nloop = strtol(optarg, NULL, 0);
            break;

        case 's':
            vecsize = strtol(optarg, NULL, 0);
            break;

        case 't':
            if (strcmp(optarg, "fixnum") == 0) {
                t = TYPE_FIXNUM;
            } else if (strcmp(optarg, "float") == 0) {
                t = TYPE_FLOAT;
            } else if (strcmp(optarg, "bignum") == 0) {
                t = TYPE_BIGNUM;
            } else {
                return 1;
            }
            break;

        case 'T':
            if (strcmp(optarg, "ruby") == 0) {
                is_included = true;
            } else if (strcmp(optarg, "lua") == 0) {
                is_included = false;
            } else {
                return 1;
            }
            break;
        }
    }

    switch (t) {
    case TYPE_FIXNUM:
        if (is_included) {
            test_fixnum<TagIncludedValue>(nloop, vecsize);
        } else {
            test_fixnum<TagSeparatedValue>(nloop, vecsize);
        }
        break;

    case TYPE_FLOAT:
        if (is_included) {
            test_float<TagIncludedValue>(nloop, vecsize);
        } else {
            test_float<TagSeparatedValue>(nloop, vecsize);
        }
        break;
    }
        
}
