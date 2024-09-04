# Pointer authentication current implementation reverse engineering

It's impmemented as a `MachineFunctionPass`.
In the public interface we have:
- an ID,
- an empty constructor,
- a `runOnMachineFunction` method
- a `getPassName` method that simply returns the pass name

As the private interface we have:
- something to do with a `BRK` instruction
- informations about the Subtarget, the Instruction info and the register info
- a function `signLR` for signing the link register
- a function `authenticateLR` to authenticate the link register
- a `emitBlend` function that blends `AddrDisc` and `IntDisc` into a misterious "result register"
- a `expandPAuthBlend` function that ..
- a `checkAuthenticatedLR` function that ..

Than the pass is initialized with the `INITIALIZE_PASS` macro and there's the implementation of the `createPass` stuff
(remember to add both of them to the `AArch64.h` file).

## `BuildPACM`

If the feature support is not known at compile time, insted we insert another similar instruction.
TODO: where is it called?

## `signLR`

This function is for the signing.
How do we get to the correct position in the code?
Probably with the `MachineBasicBlock &MBB = *MBBI->getParent();` and than simply `BuildMI` from there.
First we collect a bit of information of the target configuration.
Than we basically have 4 different if's:
- the first one is about a B-key but apparently it's dead code? The `UseBKey` is always false
- the second one get's the PC value
- than if the target supports it we emit the `paci` instruction
- and finally something that has to do with the stack destruction, i'm not sure what is about tho

## `authenticateLR`

With this function we want to authenticate the value present in the LR register.
Once again we start by collecting some informations about the target.

Than we need to move to the appropriate location in the code. How do we do that?
`MachineBasicBlock &MBB = *MBBI->getParent();`
`DebugLoc DL = MBBI->getDebugLoc();`
`MachineBasicBlock::iterator TI = MBB.getFirstInstrTerminator();`



The best way is to modify the frame-lowering code and call it a day.

Importante la questione del signing oracle

## Frame lowering

1. Some target specific informations
