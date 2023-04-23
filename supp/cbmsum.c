/* CBM checksum utility */
/* (c)opyright 2023 by Bernd Boeckmann under the BSD-3 license */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


static off_t
file_size(FILE *f)
{
    off_t pos, size;
   
    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, pos, SEEK_SET);

    return size;
}


static uint8_t *
load_file(const char *fn , off_t *size)
{
    FILE *f;
    uint8_t *buf;

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
write_file(const char *fn, const uint8_t *data, off_t len)
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


static uint8_t
calc_chksum(int algo, uint8_t *data, off_t size)
{
    uint8_t *endp = data + size;
    uint16_t chksum = 0;
    uint16_t carry = 0;

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


static uint8_t
calculate(int algo, uint8_t *data, size_t size, uint16_t addr, uint16_t chksum_addr )
{
    uint8_t chksum;

    data[chksum_addr - addr] = '\0';
    chksum = ((addr >> 8) - calc_chksum(algo, data, size)) & 0xff;
   
    printf("checksum byte at $%04X = $%02X\n",
		(unsigned)chksum_addr, (unsigned)chksum);

    return chksum;
}


static int
verify(int algo, uint8_t *data, size_t size, uint16_t addr)
{
    uint8_t correct = addr >> 8;
    uint8_t sum;

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
hexstr_to_num(const char *s, uint16_t *num)
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
      "   new    after-1983\n"
   );
}


int
main(int argc, char *argv[])
{
    uint16_t load_addr, chksum_addr;
    uint8_t *file_data = NULL;
    off_t file_size;
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
	printf("%s: ", argv[3]);
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

<<<<<<< HEAD
    return EXIT_SUCCESS;
=======
   while ( data < endp ) {
      chksum += *data++;
      if ( chksum > 0xff ) {
         chksum -= 0xff;
         carry = 1;
      }
      else {
         carry = 0;
      }
   }
   if ( algo == 2 ) chksum += carry;
   return chksum;
}

static unsigned char calculate( int algo, unsigned char *data, size_t size,
                                unsigned short addr,
                                unsigned short chksum_addr )
{
   unsigned char chksum;

   data[chksum_addr - addr] = '\0';
   chksum = ((addr >> 8) - calc_chksum( algo, data, size )) & 0xff;
   
   printf( "check-sum byte at %04X = %02X\n", (unsigned)chksum_addr,
                                              (unsigned)chksum);
   return chksum;
}

static int verify( int algo, unsigned char *data, size_t size,
                   unsigned short addr )
{
   unsigned char correct_chksum = addr >> 8;
   unsigned char chksum;

   chksum = calc_chksum( algo, data, size );
   
   printf( "data range %4X-%4X\n", (unsigned)addr,
                                   (unsigned)((addr + size - 1)) );
   if ( chksum == correct_chksum ) {
      printf( "check-sum is correct: %2X\n", chksum );
   }
   else {
      printf( "CHECK-SUM MISMATCH: is %2X, should be %2X\n",
              chksum, correct_chksum);
   }

   return 1;
}

int main( int argc, char *argv[] )
{
   unsigned short load_addr, chksum_addr;
   unsigned char *file_data = NULL;
   size_t file_size;
   int algo;

   if ( argc < 5 ) goto usage;

   /*  get checck-sum algorithm */
   if ( !strcmp( argv[2], "old" ) ) algo = 1;
   else if ( !strcmp( argv[2], "new" ) ) algo = 2;
   else {
      printf( "error: invalid algorithm\n" );
      goto error;
   }

   /* get load address */
   if ( !hexstr_to_num( argv[4], &load_addr ) ) {
      printf( "error: illegal load address\n" );
      goto error;
   }

   /* read ROM file */
   file_data = load_file( argv[3], &file_size );
   if ( !file_data ) {
      printf( "error: can not load input file\n" );
      goto error;
   }

   /* get check-sum address */
   if (  !strcmp( argv[1], "calc") || !strcmp( argv[1], "patch") ) {     
      if ( argc < 6 ) goto usage;
      if ( !hexstr_to_num( argv[5], &chksum_addr ) || 
         ( chksum_addr < load_addr || chksum_addr > load_addr + file_size - 1 ) ) {
         printf( "error: illegal check-sum address\n" );
         free( file_data );
         goto error;
      }
   }

   if ( !strcmp(argv[1], "verify" ) ) {
      verify( algo, file_data, file_size, load_addr );
   }
   else if ( !strcmp( argv[1], "calc" ) ) {
      calculate( algo, file_data, file_size, load_addr, chksum_addr );
   }
   else if ( !strcmp( argv[1], "patch" ) ) {
      if ( argc != 7 ) goto usage;
      file_data[chksum_addr - load_addr] =
         calculate( algo, file_data, file_size, load_addr, chksum_addr );
      if ( !write_file( argv[6], file_data, file_size ) ) {
         printf( "error: can not write output file\n" );
      }
      printf( "patched file is %s, %u bytes written\n", argv[6], (unsigned)file_size );
   }
   else {
      goto usage;
   }

   free( file_data );
   return EXIT_SUCCESS;
>>>>>>> c4c54f27346d80fdeb81f89e49c7c7ae17044571

usage:
    print_usage();

error:
    if (file_data)
	free(file_data);

    return EXIT_FAILURE;
}
