# lzasm
lzasm is a lightweight runtime assembler written in C++.

Its main purpose is to assemble the decompression code in [shrinkler-gba](http://github.com/tom42/shrinkler-gba).

Currently, only ARMv4T Thumb instructions are supported.
Moreover, only little endian CPUs are supported.

The name lzasm stands for Laufzeitassembler - runtime assembler in German.
lzasm has nothing to do with LZASM (Lazy Assembler, some sort of TASM clone/replacement).

# Supported platforms

The production code of lzasm uses C++20 and depends on the C++ Standard Library only.
The unit tests are written using Boost.Test, but lzasm can be used without Boost.

lzasm has been tested with the following compilers, x86 and x86-64 each:
* gcc 10.3.0
* clang 11.0 and 12.0
* Visual Studio 2019

# Basic usage
lzasm is a header-only library, so you just need to point your compiler at its include directory.
That said, the build process of lzasm uses CMake and provides a basic `install` target,
which installs the lzasm headers and makes lzasm available through CMake's `find_package` command.
For more information refer to [INSTALL.md](INSTALL.md).

Below is a short usage example. For more information refer to [USAGE.md](USAGE.md).

```c++
#include <iostream>
#include <lzasm/arm/arm32/divided_thumb_assembler.hpp>

int main()
{
    using namespace lzasm::arm::arm32;

    // Create assembler
    divided_thumb_assembler a;

    // Assemble code
    a.add(r0, r1, r2);
    a.mul(r3, r4);
    a.add(r5, 42);      // Add immediate: add r5, #42

    // Finally call link(), specifying the load address.
    // link() may throw std::runtime_error, for instance
    // if there are undefined symbols.
    bytevector program = a.link(0x1000);

    // Do whatever you like with the program
    std::cout << "Program size: " << program.size() << " bytes" << std::endl;
}
```
