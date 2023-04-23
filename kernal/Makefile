
ASM	:= P:\VARCem8\bin\vasm
AFLAGS	:= -q -p6510 -F


kernal.bin:	kernal.a65 \
		 chanio.a65 clall.a65 close.a65 declare.a65 disclaim.a65 \
		 editor1.a65 editor2.a65 editor3.a65 errhndl.a65 init.a65 \
		 irqfile.a65 load.a65 msgs.a65 open.a65 openchan.a65 \
		 patches.a65 read.a65 rs232io.a65 rs232nmi.a65 rs232rcv.a65 \
		 rs232trn.a65 save.a65 serial40.a65 tapectrl.a65 tapefile.a65 \
		 time.a65 vectors.a65 write.a65      
		$(ASM) $(AFLAGS) -s -l kernal.lst -o $@ $(DEFS) kernal.a65


# End of Makefile.
