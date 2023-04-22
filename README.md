# C64 Kernal V3 and BASIC V2 source ported to ASM6502

The assembler can be obtained from https://github.com/boeckmann/asm6502.

The original source can be found at https://github.com/mist64/cbmsrc/tree/master/.


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

The binaries are *not* the ROM files, because the BASIC binary is too large to fit into its ROM. The last $4B7 bytes of BASIC reside in the KERNAL ROM.

To generate the KERNAL and BASIC ROM files run the following after creating the binaries above. You may have to adjust the two .a65 files in the rom directory if the directory separator of your operating system is not the forward slash `/`:
```
cd rom
asm6502 kernal.a65 kernal.rom
asm6502 basic.a65 basic.rom
```
This generates `kernal.rom` and `basic.rom` inside the `rom` sub-directory. These are the ROM files.

## Adjusting the check-sum byte
The check-sum bytes are zero. You may put the original check-sums into place with your favourite hex editor:

 - Basic ROM check sum at offset $1F52 = $EC
 - Kernal ROM check sum at offset $4AC = $81

Or you may use the _cbmsum_ program provided in this repository. It is a little C program provided via the file `supp/cbmsum.c`. You can compile it with your favourite ANSI C compiler. To patch the ROMs, use it like this: 
```
cbmsum patch old kernal.rom e000 e4ac kernal.rom
cbmsum patch old basic.rom a000 bf52 basic.rom
```
The tool can also verify the check-sum of a Commodore ROM. Call it without parameters for usage info.

## Flags
Two flags may be altered in the file `kernal/kernal.a65`. If you alter one of these you will not produce an exact replica of the Kernal V3 ROM:

 - `FLAG_RRBY`: Most ROMS contain "RRBY" at $FFF6-$FFF9. These are the initials of Commodore engineers. The initials are not included in the source I based my work on, so I patched them in again. Set `FLAG_RRBY=0` to disable it.
 - `FLAG_FIX_FF7E`: The source I based my work on contains `JMP CLKHI` at $FF7D, but it is `JMP CLKLO` in the Kernal V3 ROM. FLAG_FIX_FF7E may be set to 1 if you want it to be `JMP CLKHI`.

