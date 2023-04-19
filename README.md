# C64 Kernal source ported to ASM6502

The assembler can be obtained from https://github.com/boeckmann/asm6502

The original source can be found at https://github.com/mist64/cbmsrc/tree/master/KERNAL_C64_03


## Building
Build with:
```
asm6502 kernal.a65 kernal.bin kernal.lst
```
The command creates a binary 8192 bytes in size. The first $4D3 bytes are filled by $AA in place of basic code. Otherwise this should hopefully be an exact replica of the C64 KERNAL V3 ROM.


## And so forth...
The KERNAL ROM is 8k in size and located at E000-FFFF.

There are a few things to consider:
- The C64 KERNAL ROM seems to include some parts of the BASIC interpreter at E000-E4B7, probably because it does not fit into its own ROM.
- There are holes in the ROM with fill value $AA.
- The KERNAL code starts at E5000, but from E4D3-E4FF there are 45 bytes of code originating from the source _irqfile_ for fixing various bugs.
