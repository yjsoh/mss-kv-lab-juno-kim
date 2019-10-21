# Project overview
This project aims to build a fast, scalable, and crash-consistent key-value store
on persistent memory (we name it as PMKV) using the open-source Persistent Memory Developement Kit
([PMDK](https://pmem.io/pmdk/)) library.
The purpose of this project is to get you familiar with how to program your own
data structure on persistent memory.  In doing so, you will learn a number of key concepts
about persistent memory programming and how to apply them in your code by using PMDK library.

Your task is to implement all of the following interfaces using PMDK.  Although PMDK
consists of multiple libraries, you will only need to use **libpmemobj** that supports
building a transactional object store on a persistent memory file.  Since it is written in C,
your implementation needs to be written in C as well.  The following interface is defined in
include/pmkv.h.

```
typedef struct {} pmkv;

pmkv* pmkv_open(const char *path, size_t pool_size, int recover);
void pmkv_close(pmkv *kv);
int pmkv_get(pmkv *kv, const char *key, size_t key_size, char *out_val, size_t *out_val_size);
int pmkv_put(pmkv *kv, const char *key, size_t key_size, const char *val, size_t val_size);
int pmkv_delete(pmkv *kv, const char *key, size_t key_size);
int pmkv_count_all(pmkv *kv, size_t *cnt);
int pmkv_exists(pmkv *kv, const char *key, size_t key_size);
```

TBD

Once your implementation of PMKV is completed, it will be evaluated in three categories:

- Functional correctness
- Crash-consistency
- Performance

The first part of the project is to make your PMKV functionally correct.
This means the basic operations such as `pmkv_get`, `pmkv_put`, and `pmkv_delete` operate correctly.  Our test harness
will test them with various workloads.  You should also implement `pmkv_count_all` and `pmkv_exists` for
your PMKV to pass our test cases.

The second part of the project is to make your PMKV crash-consistent.
Being crash-consistent means that your PMKV should always be in a consistent state after recovery.
Our test harness will test this by randomly crashing your program and trying recovery several times.  We will
repeat this long enough until we find any inconsistent state of your program.

The last part of the project is to optimize the performance of your PMKV.


As a reference, we provide an example implementation that uses [PMEMKV](https://github.com/pmem/pmemkv).
PMEMKV provides multiple types of engines that conform to the same common API.  Provided engines are
either volatile or persistent and vary in supporting concurrent access and sorting.  By default,
we use `cmap`, a volatile, concurrent hash map, as reference.  You can find the code in `src/pmemkv.c`.


## Getting started
For this project, we prepared a Docker image that contains all prerequisites, so the only thing you need to do
is to build and run the Docker image.  Install Docker by downloading it from the [Docker website](https://www.docker.com/).

There are a number of files and directories in this repository.  Here is a brief overview.
- At the top level, Makefile and Dockerfile help you build and run the docker image that you will use for the project.  Makefile.docker
will be copied as Makefile into the project home directory in your docker container.
- `example` contains various PMDK examples that you will find helpful to understand the library better.
- `include` contains the `pmkv.h` header file that defines the PMKV interfaces you need to implement.
- `src` contains the `pmkv.c` source file that will hold your implementation.  You need to add your implementation to this file. 
`src` also contains a PMEMKV reference implementation in `pmemkv.c`.
- `test` contains the test harness.
- `bench` contains the performance benchmark.

## Running the Docker image
First, build the docker image by doing the following:
```
$ make image
``` 
This may take for a while (~20 minutes).  If you have many cores in your machine and want to speed up the build,
you can change the value of `build_core` in the `image` recipe in the Makefile.  The default is using 8 cores.

Once the image is built, you can run it by doing the following:
```
$ make run
```
This will place you in the home directory of your project in the Docker container.

## PMDK examples
To help you better understand PMDK (and libpmemobj in particular), we provide example codes in `example` directory.
You can build and run them by doing the following:
```
$ cd example
$ make <target_example_name>
$ ./bin/<target_example_name> <args>
```
For those who are not familiar with how to use PMDK, we highly recommend you to try these examples first.  Play enough with
these examples to get confidence before jumping into the actual PMKV implementation! 

## Building PMKV
Once you are done with your own implementation, you can build it as follows:
```
$ cd src
$ make
```
This will generate a static library named `libpmkv.a` in `src` directory. This library is the core output that will be used for testing and benchmarking later on.

If you want to test the reference implementation based on PMEMKV, build it as follows:
```
$ cd src
$ make PMEMKV=1
```
This will generate the `libpmkv.a` using the PMEMKV implementation. Since PMEMKV is already a stable implementation, it will completely pass
all the test cases in `test` directory.  You can also check how it performs in `bench` directory.

## Testing
Testing PMKV involves two steps.

The first is `basic_test` that tests the functional correctness of your PMKV implementation.
The largest test case creates 2 GB of pool and inserts and searches 4 million key-value pairs.
`basic_test` currently consists of 24 test cases in total, but may be added with more test cases.

To run the `basic_test`, do the following:
```
$ cd test
$ make clean
$ make basic_test
$ ./basic_test
```

The second is `recovery_test` that tests the crash-consitency of your PMKV implementation.
TBD.


## Measuring performance
TBD
