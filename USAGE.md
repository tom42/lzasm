# lzasm usage

## Installation
lzasm is a header-only library, so you just need to point your compiler at its include directory.
That said, the build process of lzasm uses CMake and provides a basic `install` target,
which installs the lzasm headers and makes lzasm available through CMake's `find_package` command.
For more information refer to [INSTALL.md](INSTALL.md).

## Basic usage
Short usage example:

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

## Endianness
Currently code is generated for little endian CPUs. Big endian CPUs are not supported.

## Error reporting
Errors are reported by throwing `std::runtime_error`:

```c++
// This will throw std::runtime_error, since the immediate value is out of range.
a.add(r0, r1, 8);               // Error: immediate is out of range
```

## Call chaining
Functions that generate code return `*this`, so that calls can be chained.
The above example could be written as follows:

```c++
divided_thumb_assembler a;

a
.add(r0, r1, r2)
.mul(r3, r4)
.add(r5, 42);

bytevector program = a.link(0x1000);
```

## Deriving from `divided_thumb_assembler`
Another technique is to derive from `divided_thumb_assembler`.
To some people this might look better than call chaining:

```c++
class my_program : public divided_thumb_assembler
{
public:
    my_program()
    {
        add(r0, r1, r2);
        mul(r3, r4);
        add(r5, 42);
    }
};

// And then somewhere in your program, for example:
bytevector code = my_program().link(0);
```

## Labels
By default, label names are of type `std::string`:

```c++
// divided_thumb_assembler is an alias for basic_divided_thumb_assembler<std::string>
using std::string;
divided_thumb_assembler a;

a.cmp(r0, 42);
a.beq(string("some_label"));    // Label reference
a.mov(r0, 6);
a.mov(r1, 9);
a.mul(r0, r1);
a.label(string("some_label"));  // Label definition
```

Implicit conversion from `const char*` is not supported.
If it was, the integer literal `0` would be ambiguous, because it could be
either a pointer or an integer:

```c++
a.label("some_label");          // Error: cannot convert
a.add(r0, 0);                   // Reason: is 0 an integer or a null pointer?
```

As a shorthand you can use `std::string` literals:

```c++
#include <string>
using namespace std::string_literals;
// ...
a.label("another_label"s);
```

It is possible to use a different type for label names, e.g. `int`:

```c++
basic_divided_thumb_assembler<int> a;

a.cmp(r0, 42);
a.beq(symbol(0));               // Label reference
// ...
a.label(symbol(0));             // Label definition
```

For any label name type other than `std::string`, `symbol()` must be used
to define or reference a label. This is required to distinguish labels from numbers:

```c++
basic_divided_thumb_assembler<int> a;

a.add(r0, 1);                   // Adds the number 1 to r0
a.add(r0, symbol(1));           // Adds the value of symbol 1 to r0
a.label(symbol(1));             // And here symbol 1 is defined
```

**Caution**: do *not* use pointer types as label names, including `const char*`.
The assembler will just store and compare pointer addresses as label names,
which is probably not what you want.

## Literals and literal pools
Literals and literal pools are supported:
```c++
a.ldr(r0, 0x12345678);          // ldr r0, =0x12345678
a.add(r0, r0, r0);
// ...
a.pool();                       // Create literal pool here
```

Multiple literal pools can be created.
If there are any literals that have not yet been emitted by a
`pool` directive when `link()` is called, a final literal pool is
automatically created.

When a `pool` directive creates a literal pool it automatically word-aligns the pool.

However, when there are no literals that have not yet been placed in a pool,
the `pool` directive will not modify the location counter in any way:

```c++
divided_thumb_assembler a;
a.add(r0, r0, r0);              // Location counter is now 2
a.pool();                       // No unplaced literals, location counter remains 2
```

Obviously placing a literal pool in the middle of the code like that makes no sense whatsoever.

## Syntax differences from a conventional assembler
Being a C++ library, lzasm's syntax obviously differs from the syntax
of a conventional assembler:

```c++
// Immediate values do not have any prefix.
a.mov(r0, 123);                 // mov r0, #123

// and is a keyword in C++, so lzasm uses and_ for that instruction
a.and_(r0, r1);

// Register lists do not use braces.
a.push(r0);                     // push {r0}
a.push(r0, r3 - r5, lr);        // push {r0, r3 - r5, lr};
a.pop(r0 - r3, r5 - r7);        // pop {r0 - r3, r5 - r7}
a.pop(r4, pc);                  // pop {r4, pc};

// In C++, ! is right-associative, so instead of reg!, !reg must be written.
a.ldmia(!r0, r1 - r4);          // ldmia r0!, {r1 - r4}
a.stmia(!r1, r2);

// Addressing modes do not use brackets.
a.ldr(r0, r1, 0);               // ldr r0, [r1]
a.ldr(r0, r1, 20);              // ldr r0, [r1, #20]
a.ldr(r0, r1, r2);              // ldr r0, [r1, r2]
```

## Directives and pseudo instructions

### Data definition directives

#### byte, hword and word
lzasm provides data definition directives to define bytes, halfwords and words:

```c++
divided_thumb_assembler a;

// An arbitrary number of values can be
// passed to the data definition directives.
a.byte(0);
a.byte(0x11, 0x22, 0x33 /* ... */);

a.hword(0);
a.hword(0x1111, 0x2222 /* ... */);

a.word(0);
a.word(0x11111111, 0x22222222 /* ... */);

// Data definition directives can reference labels.
a.word("some_label"s);
// ...
a.label("some_label"s);
```

#### incbin
The `incbin` directive outputs a range of bytes:

```c++
std::vector<unsigned char> bytes{ 1, 2, 3 };
divided_thumb_assembler a;

// Equivalent to a.byte(1, 2, 3)
a.incbin(bytes.begin(), bytes.end());
```

### adr
The `adr` pseudo instruction loads an address into a register.
It does so by generating an `add rn, pc, immediate` instruction:

```c++
divided_thumb_assembler a;

// Generate an 'add r0, pc, immediate' instruction
// that loads the address of some_label into r0.
a.adr(r0, "some_label"s);

// ...

a.align(2);                     // The label must be word-aligned
a.label("some_label"s);
```

If the second argument passed to the `adr` pseudo instruction is
a numeric value rather than a label, then that value is assumed
to be an absolute value:

```c++
// Generate an 'add r0, pc, immediate' instruction
// that loads the address 0x400 into r0, regardless
// of the program load address passed to link().
a.adr(r0, 0x400);
```

If the address to load is not within 0 to 1020 bytes of the PC,
an exception will be thrown when `link()` is called.

### align
The `align` directive aligns the location counter to a specified
boundary, or alignment. The alignment is specified as the number
of low-order zero bits the location counter should have:

```c++
divided_thumb_assembler a;
// ...
a.align(2);                     // Align to a multiple of 4 bytes (2^2)
a.word(1, 2, 3 /* ... */);      // Create some word-aligned data
```

### ARM code generation pseudo instructions
`divided_thumb_assembler` supports Thumb instructions only, but there are a few
pseudo instructions that generate ARM code. They may be useful if your program
gets entered in ARM state and you need to switch to Thumb state yourself.

The `arm_branch` pseudo instruction generates an unconditional ARM branch instruction.
The `arm_to_thumb` pseudo instruction generates a sequence of instructions that
switch the CPU to Thumb state.

For example, assume the Gameboy Advance, where the entry point of a game ROM
is right at the beginning of the ROM and is executed in ARM state. Moreover,
there is exactly 4 bytes of space available for code at the entry point,
so all you can do there is branch to the actual beginning of the ROM's code:

```c++
divided_thumb_assembler a;

// Entry point, provides just enough room
// for an unconditional ARM branch instruction
a.arm_branch("code_start"s);

// ROM header (192 bytes, most of them omitted for brevity)
a.byte(0x24, 0xff, 0xae, 0x51 /* ... */);

// Actual code starts here, switch to Thumb state
a.label("code_start"s);
a.arm_to_thumb(r0);
```

The `arm_to_thumb` pseudo instruction takes a single register operand which
may be freely chosen. It generates the following instruction sequence:

```assembly
    add r, pc, #1               # r = thumb_start + 1
    bx r                        # Switch to Thumb state
thumb_start:
```

Obviously the original value of `r` is lost.

## Functions

### current_lc

The `current_lc` function returns the current location counter:

```c++
divided_thumb_assembler a;
// Emit some code
a.nop();
a.nop();
assert(a.current_lc() == 42);
```
