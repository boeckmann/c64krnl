# Makefile to build the C64 Kernal V3 and BASIC V2 ROMs

ASM	:= asm6502
AFLAGS	:= -q
CBMSUM	:= ../supp/cbmsum

MD5	:= md5 -q

KERNAL_MD5 := 39065497630802346bce17963f13c092
BASIC_MD5  := 57af4ae21d4b705c2991d98ed5c1f7b8

ifndef VICE
VICE = 0
endif

.PHONY: all roms verify clean

all:	roms

roms:	rom/kernal.rom rom/basic.rom

verify: roms
	@test `$(MD5) $(MD5_ARGS) rom/kernal.rom` = $(KERNAL_MD5)
	@test `$(MD5) $(MD5_ARGS) rom/basic.rom` = $(BASIC_MD5)

kernal/kernal.bin: kernal/*.a65
	@cd kernal; $(ASM) $(AFLAGS) kernal.a65 -o kernal.bin -l ../listings/kernal.lst

basic/basic.bin: basic/*.a65
	@cd basic; $(ASM) $(AFLAGS) basic.a65 -o basic.bin -l ../listings/basic.lst

rom/kernal.rom: rom/kernal.a65 kernal/kernal.bin basic/basic.bin
	@cd rom; $(ASM) $(AFLAGS) kernal.a65 -o kernal.rom VICE=$(VICE)
ifeq ($(VICE), 0)
	@cd rom; $(CBMSUM) patch old kernal.rom e000 e4ac kernal.rom
endif

rom/basic.rom: rom/basic.a65 basic/basic.bin
	@cd rom; $(ASM) $(AFLAGS) basic.a65 -o basic.rom
	@cd rom; $(CBMSUM) patch old basic.rom a000 bf52 basic.rom

clean:
	@rm -f kernal/*.bin
	@rm -f basic/*.bin
	@rm -f kernal/*.lst
	@rm -f basic/*.lst
	@rm -f rom/*.rom
# DOS creates upper case files
	@rm -f kernal/*.BIN
	@rm -f basic/*.BIN
	@rm -f kernal/*.LST
	@rm -f basic/*.LST
	@rm -f rom/*.ROM
