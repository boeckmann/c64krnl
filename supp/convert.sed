# helper script to convert the source to ASM6502 syntax

s/\.SKI.*//;
s/\.PAG.*//;
s/)Y/),Y/;
s/ROR A /ROR /;
s/ROL A /ROL /;
s/ASL A /LSL /;
s/ASR A /ASR /;
s/LSR A /LSR /;
s/\.BYT /.BYTE /;
s/\.WOR /.WORD /;
s/\.END//
s/\.LIB /.include "/;
s/\*=\*/.org @/;
s/\*=/.org /;
