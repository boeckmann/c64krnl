# C64 Kernal V3 and BASIC V2 source ported to ASM6502

The assembler can be obtained from https://github.com/boeckmann/asm6502

The original source can be found at https://github.com/mist64/cbmsrc/tree/master/


## Building
Build the KERNAL and BASIC binary files:
```
cd kernal
asm6502 kernal.a65 kernal.bin kernal.lst
cd ../basic
asm6502 basic.a65 basic.bin basic.lst
cd ..
```
There are now `kernal/kernal.bin` and `basic/basic.bin` files and listing files `kernal/kernal.lst` and `basic/basic.lst`.

The binaries are *not* the ROM files, because the BASIC binary is too large to fit into ROM. The last $4B7 bytes of BASIC reside in the KERNAL ROM.

To generate the KERNAL and BASIC ROM files run the following after creating the binaries above. You may have to adjust the two .a65 files in the rom directory if the directory separator of your operating system is not the forward slash `/`:
```
cd rom
asm6502 kernal.a65 kernal.rom
asm6502 basic.a65 basic.rom
```
This generates `kernal.rom` and `basic.rom` inside the `rom` sub-directory. These are the ROM files.

The check sum bytes are zero. You may put the original check sums into place with your favourite hex editor:

 - Basic ROM check sum at offset $1F52 = $EC
 - Kernal ROM check sum at offset $4AC = $81

## Variations

- Most ROMS contain "RRBY" at $FFF6-$FFF9, these are the initials of authors. But this is not included in the original source I based my work on. I included a flag FLAG_RRBY in kernal.a65 to put them in (on by default).
