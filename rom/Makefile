
ASM	:= P:\VARCem8\bin\vasm
AFLAGS	:= -q -p6510 -F

ROMS	= kernal.rom basic.rom


all:		$(ROMS)


kernal.rom:	kernal.a65
		$(ASM) $(AFLAGS) -o $@ $(DEFS) kernal.a65

basic.rom:	basic.a65
		$(ASM) $(AFLAGS) -o $@ $(DEFS) basic.a65


crc:		$(ROMS)
		zcrc kernal.rom good/kernal.901227-03.bin
		zcrc basic.rom good/basic.901226-01.bin


# End of Makefile.
