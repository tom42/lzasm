# lzasm installation

lzasm is a header-only library, so you just need to point your compiler at its include directory.
That said, the build process of lzasm uses CMake and provides a basic `install` target,
which installs the lzasm headers and makes lzasm available through CMake's `find_package` command.

## Installation from source code using CMake

To install lzasm from source code using CMake, do:

```
$ cd top-level-directory-of-lzasm-source-code
$ cmake .
$ make
$ make install
```

This will install the lzasm headers into `/usr/local/include/lzasm`
and its find_package scripts into `/usr/local/lib/cmake/lzasm`.

To uninstall everything, do:

```
$ cd top-level-directory-of-lzasm-source-code
$ make uninstall
```

To install into a directory other than `/usr/local`, instead as shown above run CMake as follows:

```
$ cmake ../.. -DCMAKE_INSTALL_PREFIX=/home/tom/local
```

Now, a project may use lzasm as follows:

```cmake
find_package(lzasm CONFIG REQUIRED)
add_executable(program-using-lzasm main.cpp)

# Installed include path will be automatically added
target_link_libraries(program-using-lzasm lzasm)
```
