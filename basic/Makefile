
ASM	:= vasm
AFLAGS	:= -q -p6510 -F


basic.bin:	basic.a65 \
		 code1.a65 code2.a65 code3.a65 code4.a65 code5.a65 \
		 code6.a65 code7.a65 code8.a65 code9.a65 code10.a65 \
		 code11.a65 code12.a65 code13.a65 code14.a65 code15.a65 \
		 code16.a65 code17.a65 code18.a65 code19.a65 code20.a65 \
		 code21.a65 code22.a65 code23.a65 code24.a65 code25.a65 \
		 code26.a65 declare.a65 disclaim.a65 init.a65 nolist.a65 \
		 token2.a65 tokens.a65 trig.a65
		$(ASM) $(AFLAGS) -s -l basic.lst -o $@ $(DEFS) basic.a65


clean:
		@-del *.lst 2>NUL
		@-del *.bin 2>NUL


# End of Makefile.
