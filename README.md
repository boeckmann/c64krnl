# C64 Kernal V3 and BASIC V2 source ported to ASM6502

This is a port of the Commodore C64 Kernal version 3 and BASIC version 2 to the ASM6502 assembler. It can also be built with the not yet released VASM by Fred N. van Kempen. The original source can be found at https://github.com/mist64/cbmsrc/tree/master/.

## Requirements

 - The assembler can be obtained from https://github.com/boeckmann/asm6502.
 - A MD5 checksum generator is needed if you want to compare your builds with known to be good ROMs.
 - The Commodore ROMs contain a one-byte checksum at somewhat random positions. The _cbmsum_ utility contained in this repository can generate these checksums and put them into place. Binaries are provided for Windows, DOS, and OS/2 (see releases).

## ROM checksums
Here are MD5 checksums of the original ROMs:
```
MD5 (kernal.rom) = 39065497630802346bce17963f13c092
MD5 (basic.rom) = 57af4ae21d4b705c2991d98ed5c1f7b8
```

## Building by Hand
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

To generate the KERNAL and BASIC ROM files run the following after creating the binaries above. You may have to adjust the two .a65 files in the rom directory if the directory separator of your operating system is not the forward slash /:
```
cd rom
asm6502 kernal.a65 kernal.rom
asm6502 basic.a65 basic.rom
```
This generates `kernal.rom` and `basic.rom` inside the `rom` sub-directory. These are the ROM files.

### Adjusting the checksum byte
The checksum bytes are zeroed in the freshly built ROM files. You may put the correct checksums into place by using the _cbmsum_ utility provided in this repository. If you do not want to download it, you may build it by yourself with every ANSI conformant C compiler.  The tool can also verify the checksum of a Commodore ROM. Call it without parameters for usage info.

To patch the ROMs to include the checksum run:
```
cd rom
../supp/cbmsum patch old kernal.rom e000 e4ac kernal.rom
../supp/cbmsum patch old basic.rom a000 bf52 basic.rom
```

You may also put the checksums into place with your favourite hex editor. But you may place wrong checksums if the ROMs do not match the original ROMs _exactly_.

 - Basic ROM check sum at offset $1F52 = $EC
 - Kernal ROM check sum at offset $4AC = $81

### Flags
Two flags may be altered in the file `kernal/kernal.a65`. If you alter one of these you will not produce an exact replica of the Kernal V3 ROM:

 - `FLAG_RRBY`: Most ROMS contain "RRBY" at $FFF6-$FFF9. These are the initials of Commodore engineers. The initials are not included in the source I based my work on, so I patched them in again. Set `FLAG_RRBY=0` to disable it.
 - `FLAG_FIX_FF7E`: The source I based my work on contains `JMP CLKHI` at $FF7D, but it is `JMP CLKLO` in the Kernal V3 ROM. FLAG_FIX_FF7E may be set to 1 if you want it to be `JMP CLKHI`.

## Building with UNIX Makefile
First build the _cbmsum_ utility by invoking _make_ in the `supp` sub-directory.

Adapt `Makefile.asm6502` and call it via `make -f Makefile.asm6502` to build the ROMs. If you have not altered the sources you may compare the checksums of your ROMs via `make -f Makefile.asm6502 verify`.
