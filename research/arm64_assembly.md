# ARM_64 assembly

Instructions in ARM assembly language are all 4 bytes long, so you can deduce the program size just by counting how many
instructions there are in the program.

The registers that we care about are 64 bits long and are named from x0 to x30 (if insted we want to refer to their
lower 32 bits instead of the full 64 bits we can use w0 to w30) and then there is the **__sp__** register which is the
stack pointer.

## Stack pointer register

It's called __sp__ and it's used to maintain the function call stack. It points to the bottom of the stack, which grows 
“down” (toward lower addresses).

What you need to remember is that the stack starts from a high address and grows towards a lower address (this is not 
always the case, it depends on the architecture).

## First example

```c
struct Vec2 {
    int64_t x;
    int64_t y;
    void debugPrint() const;
};

int64_t normSquared(Vec2 v) {
    v.debugPrint();
    return v.x * v.x + v.y * v.y;
}
```

And the corresponding assembly code:
```asm 
sub     sp, sp, #32
stp     x29, x30, [sp, #16]
add     x29, sp, #16
stp     x0, x1, [sp]
mov     x0, sp
bl      Vec2::debugPrint() const
ldp     x8, x9, [sp]
ldp     x29, x30, [sp, #16]
mul     x8, x8, x8
madd    x0, x9, x9, x8
add     sp, sp, #32
ret
```
Here's the breakdown:
-  `sub sp, sp, #32` instruction is making space for four 64-bit integers on the stack by subtracting 32 (4*32) from the 
stack pointer. Why do we need space for four 64-bit integers? Two of them are needed since inside the `normSquared`
function we are calling another function we need to save the return address and the frame pointer (x29 and x30 registers
). The other two are needed since the function takes as a parameter a `Vec2` object which is 16 bytes long (2 64-bit 
integers) abd we need to reserve space for it. **Remember that the stack is always decremented by a multiple of 16**.
- `stp x29, x30, [sp, #16]` is storing a pair of registers: it is saving the old **frame pointer** (x29) and link 
register x30 (it contains the return address) on the stack starting at the address `sp + 16` (we decremented the stack
pointer by 32 bytes, so the address of the first 64-bit integer is `sp + 16`).
- We than compute the new frame pointer with `add x29, sp, #16;`: with this instruction the **function prologue** is
completed.

At this point the code stars to execute the function body:
- `stp x0, x1, [sp]` is storing the first and second arguments to normSquared, which are v.x and v.y, to the stack (the
calling convention for ARM64 is that the first 8 arguments are passed in registers x0 to x7, and the rest are passed 
directly on the stack).
- `mov x0, sp` is putting a pointer to v in x0 to be passed to the function `Vec2::debugPrint()` with `bl` (the `bl`
instruction saves the return address in the register x30).

After the `Vec2::debugPrint()` function has returned, we load the pair of registers x8 and x9 with v.x and v.y from the
stack. We also restore the old values of the frame pointer and stack pointer. Then, we have the same `mul` and `madd`
instructions as in the previous example. Finally, we add `sp, sp, #32` to clean up the 32 bytes of stack space we 
allocated at the start of our function (called the function epilogue) and then return to our caller with `ret`.

## Second example

```c
#include <cstdio>
#include <cstring>
#include <memory>

void copyUppercase(char *dest, const char *src);

constexpr size_t MAX_STACK_ARRAY_SIZE = 1024;

void printUpperCase(const char *s) {
    auto sSize = strlen(s);
    if (sSize <= MAX_STACK_ARRAY_SIZE) {
        char temp[sSize + 1];
        copyUppercase(temp, s);
        puts(temp);
    } else {
        // std::make_unique_for_overwrite is missing on Godbolt.
        std::unique_ptr<char[]> temp(new char[sSize + 1]);
        copyUppercase(temp.get(), s);
        puts(temp.get());
    }
}
```

And here's the generated assembly code:

```asm
printUpperCase(char const*):
        stp     x29, x30, [sp, #-32]!
        str     x19, [sp, #16]
        mov     x29, sp
        sub     sp, sp, #80
        stur    x0, [x29, #-8]
        ldur    x0, [x29, #-8]
        bl      strlen
        stur    x0, [x29, #-16]
        ldur    x8, [x29, #-16]
        subs    x8, x8, #1024
        b.hi    .LBB0_2
        b       .LBB0_1
.LBB0_1:
        ldur    x8, [x29, #-16]
        add     x8, x8, #1
        mov     x9, sp
        stur    x9, [x29, #-24]
        lsr     x9, x8, #0
        add     x9, x9, #15
        and     x10, x9, #0xfffffffffffffff0
        mov     x9, sp
        subs    x0, x9, x10
        mov     sp, x0
        stur    x0, [x29, #-64]
        stur    x8, [x29, #-32]
        ldur    x1, [x29, #-8]
        bl      copyUppercase(char*, char const*)
        ldur    x0, [x29, #-64]
        bl      puts
        ldur    x8, [x29, #-24]
        mov     sp, x8
        b       .LBB0_6
.LBB0_2:
        ldur    x8, [x29, #-16]
        add     x0, x8, #1
        bl      operator new[](unsigned long)
        mov     x1, x0
        sub     x0, x29, #40
        stur    x0, [x29, #-72]
        bl      std::unique_ptr<char [], std::default_delete<char []>>::unique_ptr<char*, std::default_delete<char []>, void, bool>(char*)
        ldur    x0, [x29, #-72]
        bl      std::unique_ptr<char [], std::default_delete<char []>>::get() const
        ldur    x1, [x29, #-8]
        bl      copyUppercase(char*, char const*)
        b       .LBB0_3
.LBB0_3:
        sub     x0, x29, #40
        bl      std::unique_ptr<char [], std::default_delete<char []>>::get() const
        bl      puts
        b       .LBB0_4
.LBB0_4:
        sub     x0, x29, #40
        bl      std::unique_ptr<char [], std::default_delete<char []>>::~unique_ptr() [base object destructor]
        b       .LBB0_6
        stur    x0, [x29, #-48]
        mov     w8, w1
        stur    w8, [x29, #-52]
        sub     x0, x29, #40
        bl      std::unique_ptr<char [], std::default_delete<char []>>::~unique_ptr() [base object destructor]
        b       .LBB0_7
.LBB0_6:
        mov     sp, x29
        ldr     x19, [sp, #16]
        ldp     x29, x30, [sp], #32
        ret
.LBB0_7:
        ldur    x0, [x29, #-48]
        bl      _Unwind_Resume

__clang_call_terminate:
        stp     x29, x30, [sp, #-16]!
        mov     x29, sp
        bl      __cxa_begin_catch
        bl      std::terminate()

DW.ref.__gxx_personality_v0:
        .xword  __gxx_personality_v0
```

Let's break it down:
```asm
printUpperCase(char const*):
    stp     x29, x30, [sp, #-32]!  // Save the old frame pointer (x29) and link register (x30) on the stack and adjust the stack pointer.
    str     x19, [sp, #16]         // Save the x19 register on the stack (x19 will be used later).
    mov     x29, sp                // Update the frame pointer to the current stack pointer.
    sub     sp, sp, #80            // Allocate 80 bytes on the stack for local variables.
```
This is the **function prologue**:
- `stp x29, x30, [sp, #-32]!`: Stores the previous frame pointer and link register on the stack and adjusts the stack pointer downwards.
- `str x19, [sp, #16]`: Saves the x19 register on the stack (why? Because it's a convention, it's one of the registers
the callee needs to preserve [ARM Procedure Call Standard](https://developer.arm.com/documentation/102374/0102/Procedure-Call-Standard)).
- `mov x29, sp`: Updates the frame pointer to the new stack pointer.
- `sub sp, sp, #80`: Allocates space on the stack for local variables.

Then the code **handles the string length**,
```asm
 stur    x0, [x29, #-8]         // Store the input pointer (s) at [x29 - 8].
 ldur    x0, [x29, #-8]         // Load the value of s (from [x29 - 8]) back into x0.
 bl      strlen                 // Call strlen to get the length of the string s.
 stur    x0, [x29, #-16]        // Store the result (sSize) in [x29 - 16].
```
- `stur x0, [x29, #-8]`: Stores the input pointer s at [x29 - 8].
- `ldur x0, [x29, #-8]`: Loads s back into x0 (for passing it to strlen).
- `bl strlen`: Calls strlen to compute the length of s.
- `stur x0, [x29, #-16]`: Stores the length of s (i.e., sSize) at [x29 - 16].

Next, the code **checks if the string is small enough to fit on the stack**:
```asm
ldur    x8, [x29, #-16]        // Load sSize from [x29 - 16] into x8.
subs    x8, x8, #1024          // Subtract 1024 (MAX_STACK_ARRAY_SIZE) from sSize and set flags based on the result.
b.hi    .LBB0_2                // If sSize > 1024, jump to .LBB0_2 (heap allocation case).
b       .LBB0_1                // Otherwise, jump to .LBB0_1 (stack allocation case).
```
- `subs x8, x8, #1024`: Compares sSize to 1024.
- `b.hi .LBB0_2`: If sSize is greater than 1024, branch to the heap allocation block.
- `b .LBB0_1`: Otherwise, branch to the stack allocation block.

In the **stack allocation block**:
```asm
.LBB0_1:
    ldur    x8, [x29, #-16]        // Load sSize from [x29 - 16] into x8.
    add     x8, x8, #1             // Compute sSize + 1 (for the null terminator).
    mov     x9, sp                 // Save the current stack pointer in x9.
    stur    x9, [x29, #-24]        // Save the stack pointer in [x29 - 24] (backup).
    lsr     x9, x8, #0             // Prepare for stack alignment.
    add     x9, x9, #15            // Align size to 16 bytes (add 15 to round up).
    and     x10, x9, #0xfffffffffffffff0 // Align the size to the nearest multiple of 16.
    mov     x9, sp                 // Load the current stack pointer into x9.
    subs    x0, x9, x10            // Allocate space for the temporary array by adjusting the stack pointer.
    mov     sp, x0                 // Update the stack pointer.
    stur    x0, [x29, #-64]        // Save the adjusted stack pointer (for the temporary array) at [x29 - 64].
    stur    x8, [x29, #-32]        // Save sSize + 1 at [x29 - 32].
    ldur    x1, [x29, #-8]         // Load the original string pointer (s) into x1.
    bl      copyUppercase(char*, char const*) // Call copyUppercase(temp, s).
    ldur    x0, [x29, #-64]        // Load the pointer to the temporary array into x0.
    bl      puts                   // Print the uppercase string.
    ldur    x8, [x29, #-24]        // Restore the original stack pointer from [x29 - 24].
    mov     sp, x8                 // Restore the stack pointer to its original value.
    b       .LBB0_6                // Jump to function epilogue.
```
- `add x8, x8, #1`: Computes sSize + 1 to account for the null terminator.
- `storing stack and computing alignment`: This block computes the required stack space for the temp array and adjusts the stack pointer accordingly.
- `bl copyUppercase(char, char const*)`: Calls copyUppercase with the stack-allocated temp buffer.
- `bl puts`: Prints the uppercase string using puts.
- `mov sp, x8`: Restores the stack pointer after using the temporary buffer.

In the **heap allocation block**:
```asm
.LBB0_2:
    ldur    x8, [x29, #-16]        // Load sSize from [x29 - 16] into x8.
    add     x0, x8, #1             // Compute sSize + 1.
    bl      operator new[](unsigned long) // Allocate memory on the heap for the array.
    mov     x1, x0                 // Move the allocated memory pointer into x1 (will be passed to copyUppercase).
    sub     x0, x29, #40           // Prepare to construct the unique_ptr with the allocated memory.
    stur    x0, [x29, #-72]        // Store the unique_ptr base address.
    bl      std::unique_ptr<char [], std::default_delete<char []>>::unique_ptr<char*, std::default_delete<char []>, void, bool>(char*) // Construct unique_ptr.
    ldur    x0, [x29, #-72]        // Load the unique_ptr base address into x0.
    bl      std::unique_ptr<char [], std::default_delete<char []>>::get() const // Get the raw pointer from the unique_ptr.
    ldur    x1, [x29, #-8]         // Load the original string pointer (s) into x1.
    bl      copyUppercase(char*, char const*) // Call copyUppercase(temp.get(), s).
    b       .LBB0_3                // Jump to the next block.
```
- `bl operator new[]`: Allocates memory on the heap for the temp array.
- `unique_ptr construction`: The unique pointer is constructed to manage the allocated memory.
- `bl copyUppercase(char, char const*)`: Calls copyUppercase with the heap-allocated temp buffer.

Now, the code **handles the printing and the heap allocation cleanup**:
```asm
.LBB0_3:
    sub     x0, x29, #40           // Load the unique_ptr base address into x0.
    bl      std::unique_ptr<char [], std::default_delete<char []>>::get() const // Get the raw pointer from the unique_ptr.
    bl      puts                   // Print the uppercase string using the pointer.
    b       .LBB0_4                // Jump to the next block.

.LBB0_4:
    sub     x0, x29, #40           // Prepare to destroy the unique_ptr.
    bl      std::unique_ptr<char [], std::default_delete<char []>>::~unique_ptr() // Destroy the unique_ptr (clean up heap memory).
    b       .LBB0_6                // Jump to function epilogue.
```
- `bl puts`: Prints the uppercase string using the heap-allocated buffer.
- `unique_ptr destruction`: Ensures that the heap memory is properly deallocated when the unique_ptr goes out of scope.

Finally, the **function epilogue**:
```asm
.LBB0_6:
    mov     sp, x29                // Restore the stack pointer to the frame pointer.
    ldr     x19, [sp, #16]         // Restore the x19 register.
    ldp     x29, x30, [sp], #32    // Restore the frame pointer (x29) and link register (x30), and deallocate the frame.
    ret                            // Return from the function.
```
- `mov sp, x29`: Restores the stack pointer.
- `ldp x29, x30, [sp], #32`: Restores the frame pointer and link register and deallocates the stack space used by this function.
- `ret`: Returns from the function.
