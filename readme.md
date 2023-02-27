# bench-bigint

Demo project to benchmark boost::multiprecision::cpp_int-s with Fermat's test.

## Build

```bash
$ mkdir build && cd build
$ meson setup --buildtype release
$ meson compile
# from sympy import randprime
# from sympy.ntheory import factorint
# v = [randprime(2**40, 2**41-1) for _ in range(2)]
# v[0]*v[1]
# factorint(v[0]*v[1])
$ time ./bench-bigint 3717308959939779046720901
n = 3717308959939779046720901
a = 1979844013907
b = 1877576684743
./bench-bigint 3717308959939779046720901  35.11s user 0.00s system 99% cpu 35.152 total
```
