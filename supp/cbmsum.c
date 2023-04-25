/* CBM checksum utility */
/* (c)opyright 2023 by Bernd Boeckmann under the BSD-3 license */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char u8;
typedef unsigned short u16;

static size_t
file_size(FILE *f)
{
    size_t pos, size;
   
    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, pos, SEEK_SET);

    return size;
}

static u8 *
load_file(const char *fn, size_t *size)
{
    FILE *f;
    u8 *buf;

    f = fopen(fn, "rb");
    if (f == NULL)
	return NULL;

    *size = file_size(f);
    buf = malloc(*size);
    if (buf == NULL)
	return NULL;

    (void)fread(buf, *size, 1, f);
    (void)fclose(f);   

    return buf;
}


static int
write_file(const char *fn, const u8 *data, size_t len)
{
    FILE *f = fopen(fn, "wb");
    if (f == NULL)
	return 0;

    if (fwrite(data, len, 1, f) == 0) {
	(void)fclose(f);
	return 0;
    }

    (void)fclose(f);

    return 1;
}


static u8
calc_chksum(int algo, const u8 *data, size_t size)
{
    const u8 *endp = data + size;
    u16 chksum = 0;
    u16 carry = 0;

    while (data < endp) {
	chksum += *data++;
	if ( chksum > 0xff ) {
		chksum -= 0xff;
		carry = 1;
	} else
		carry = 0;
    }

    if (algo == 2)
	chksum += carry;

    return chksum;
}


static u8
calculate(int algo, u8 *data, size_t size, u16 addr, u16 chksum_addr )
{
    u8 chksum;

    data[chksum_addr - addr] = '\0';
    chksum = ((addr >> 8) - calc_chksum(algo, data, size)) & 0xff;
   
    printf("checksum byte at $%04X = $%02X\n",
		(unsigned)chksum_addr, (unsigned)chksum);

    return chksum;
}


static int
verify(int algo, const u8 *data, size_t size, u16 addr)
{
    u8 correct = addr >> 8;
    u8 sum;

    sum = calc_chksum(algo, data, size);
   
    printf("Data range $%4X-$%4X\n",
	(unsigned)addr, (unsigned)((addr + size - 1)));

    if (sum == correct)
	printf("Checksum is correct: $%2X\n", sum);
    else
	printf("CHECKSUM MISMATCH: is $%2X, should be $%2X\n", sum, correct);

    return (sum == correct) ? 1 : 0;
}


static int
hexstr_to_num(const char *s, u16 *num)
{
    int last_num = 0;
    int digits = 0;
    char c = tolower(*s);

    *num = 0;

    while (c) {
	if (!isdigit(c) &&  (c < 'a' || c > 'f'))
		return 0;
	*num = (*num << 4) + ((c < 'a') ? c - '0' : c - 'a' + 10);
	if (*num < last_num)
		return 0;
	last_num = *num;
	c = tolower(*++s);
	digits++;
    }

    return digits <= 4;
}


static void
print_usage(void)
{
    printf(
      "CBMSUM - checksum-byte utility for Commodore ROMs\n\n"
      "commands:\n"
      "  verify <algorithm> <in-file> <load-address-hex>\n"
      "         => verify if the checksum of a ROM file is correct\n\n"
      "  calc <algorithm> <in-file> <load-address-hex> <chksum-address>\n"
      "         => calculate checksum-byte by treating ROM at chksum-address = 0\n\n"
      "  patch <algorithm> <in-file> <load-address-hex> <chksum-address> <out-file>\n"
      "         => calculate and write checksum to ROM file\n\n"
      "algorithms:\n"
      "   old    pre-1983 (C64 BASIC, KERNAL)\n"
      "   new    after-1983\n");
}


int
main(int argc, char *argv[])
{
    u16 load_addr, chksum_addr;
    u8 *file_data = NULL;
    size_t file_size;
    int algo;

    if (argc < 5)
	goto usage;

    /*  get checck-sum algorithm */
    if (! strcmp(argv[2], "old")) algo = 1;
    else if (! strcmp(argv[2], "new")) algo = 2;
    else {
	fprintf(stderr, "Error: invalid algorithm\n" );
 	goto error;
    }

    /* get load address */
    if (! hexstr_to_num(argv[4], &load_addr)) {
	fprintf(stderr, "Error: invalid load address\n");
	goto error;
    }

    /* read ROM file */
    file_data = load_file(argv[3], &file_size);
    if (file_data == NULL) {
	fprintf(stderr, "Error: can not load input file\n");
	goto error;
    }

    /* get check-sum address */
    if (!strcmp(argv[1], "calc") || !strcmp(argv[1], "patch")) {     
	if (argc < 6 )
		goto usage;

	if (!hexstr_to_num(argv[5], &chksum_addr) || 
	    (chksum_addr < load_addr || chksum_addr > load_addr + file_size - 1 )) {
		fprintf(stderr, "Error: invalid checksum address\n");
		free(file_data);
		goto error;
	}
    }

    if (! strcmp(argv[1], "verify")) {
	verify(algo, file_data, file_size, load_addr);
    } else if (! strcmp(argv[1], "calc")) {
	printf("%s: ", argv[3]);
	calculate(algo, file_data, file_size, load_addr, chksum_addr);
    } else if (! strcmp(argv[1], "patch")) {
	if (argc != 7)
		goto usage;
	printf("%s: ", argv[6]);
	file_data[chksum_addr - load_addr] = calculate(algo,
						       file_data,
						       file_size,
						       load_addr,
						       chksum_addr);
	if (! write_file(argv[6], file_data, file_size)) {
		fprintf(stderr, "Error: can not write output file\n");
	}
    } else
	goto usage;

    free(file_data);

    return EXIT_SUCCESS;

usage:
    print_usage();

error:
    if (file_data != NULL)
	free(file_data);

    return EXIT_FAILURE;
}
