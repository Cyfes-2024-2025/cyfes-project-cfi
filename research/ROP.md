## ROP

ROP it's a technique for binary exploitation.
It can bypass a series of protections (for example _ASLR_ and _code signing_) since with ROP we execute code that is
already in the _TEXT_ segment of the program (that's executable by default), bypassing protections like the one thankt
to which we cannot execute code directly from the stack.

ROP is also able to bypass ASLR if the executable is not compiled as a _Position indipendent executable_ since the code
position will remain the same between executions, effectively making ASLR useless.

### ROP gadgets

The whole attack is based on the concept of _**ROP gadgets**_: a sequence of instructions usually ending with a _ret_
that are already in the program.
By concatenating multiple gadgets it's possible to change the execution flow.
