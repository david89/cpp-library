# string_view

## Run tests
```
mkdir build
cd build && cmake ..
make
./string_view_test
```

## Warnings
* This implementation of string_view requires C++14.
* It depends on googletest.
* Very experimental.

## TODOs
* Add more comprehensive tests.
* Add tests to make sure that the string_view library has a similar behavior to the std::string library.
* Improve performance of single character operations (we could take advantage of memchr).
* Add benchmarks for performance tests.
