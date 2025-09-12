# build-your-own-lisp-sandbox
http://www.buildyourownlisp.com/contents, but i'm doing it in higher level languages (c++, rust, maybe ruby)

## current state

### c++
    - done
        - done with chap 8, onto chap 9
            - going to hand roll parser before going onto chap 10
        - gdb and valgrind setup, no mem leaks
    - todo
        - add code coverage
        - add testing
            - unit test
            - integration test
        - transition to CMake
        - add profiling
        - add clang++ sanitizer checks as alternative to valgrind
        - reduce temp vals usage by writing custom assign / cons, use emplace
            - find a way to measure heap?

### rust
    - to start
        - start while working on C++? or after done?
    - work as if C from book? or adapt C++ version?
        - import C++ modules while scale out rust version?
            - ex: parser

### stretch
    - higher level lang like python, import c++ / rust modules using pybind11 or pyo3
    - make a very pretty ruby version
    - make a golang version
    - have benchmarks between versions
        - need external profiler
        - need sample i/o, a way to measure wall time
        - prod vs debug builds?
