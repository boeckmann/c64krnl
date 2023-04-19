# C64 Kernal source ported to ASM6502

The assembler can be obtained from https://github.com/boeckmann/asm6502

The original source can be found at https://github.com/mist64/cbmsrc/tree/master/KERNAL_C64_03


## Building
Build with:
```
asm6502 kernal.a65 kernal.bin kernal.lst
```
The command creates a binary 8192 bytes in size. The first $4D3 bytes are filled by $AA. The original ROMS contain code of the BASIC interpreter in this range. Otherwise this should hopefully be an exact replica of the C64 KERNAL V3 ROM.

## Variations

- Most ROMS contain "RRBY" at $FFF6-$FFF9, these are the initials of authors. But this is not included in the original source I based my work on. I included a flag FLAG_RRBY in kernal.a65 to put them in (on by default).
