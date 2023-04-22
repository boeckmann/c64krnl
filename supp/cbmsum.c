/* CBM check-sum utility */
/* (c)opyright 2023 by Bernd Boeckmann under the BSD-3 license */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void print_usage( void )
{
	printf(
		"CBMSUM - check-sum byte utility for Commodore ROMs\n\n"
		"commands:\n"
		"  verify <algorithm> <in-file> <load-address-hex>\n"
		"         => verify if the check-sum of a ROM file is correct\n\n"
		"  calc <algorithm> <in-file> <load-address-hex> <chksum-address>\n"
		"         => calculate check-sum byte by treating ROM at chksum-address = 0\n\n"
		"  patch <algorithm> <in-file> <load-address-hex> <chksum-address> <out-file>\n"
		"         => calculate and write check-sum to ROM file\n\n"
		"algorithms:\n"
		"   old    pre-1983 (C64 BASIC)\n"
		"   new    after-1983 (C64 KERNAL)\n"
	);
}

static int hexstr_to_num( char *s, unsigned short *num )
{
	int last_num = 0;
	int digits = 0;
	char c = tolower( *s );
	*num = 0;

	while ( c ) {
		if ( !isdigit( c ) && ( c < 'a' || c > 'f') ) return 0;
		*num = (*num << 4) + ( ( c < 'a' ) ? c - '0' : c - 'a' + 10 );
		if ( *num < last_num ) return 0;
		last_num = *num;
		c = tolower( *(++s) );
		digits++;
	}

	return digits <= 4;
}

static size_t file_size( FILE *f )
{
   long pos, size;
   
   pos = ftell( f );
   fseek( f, 0, SEEK_END );
   size = ftell(f);
   fseek( f, pos, SEEK_SET );

   return size;
}

static unsigned char * load_file( const char *filename , size_t *filesize )
{
	FILE *f;
	unsigned char *buf;

	f = fopen( filename, "rb" );
	if ( !f ) return NULL;
	*filesize = file_size( f );
	buf = malloc( *filesize );
	if ( !buf ) return NULL;
	fread( buf, 1, *filesize, f );
	fclose( f );	
	return buf;
}

static int write_file (const char *fn, const unsigned char *data, int len )
{
   FILE *f = fopen( fn, "wb" );
   if ( !f ) return 0;
   if ( fwrite( data, len, 1, f ) == 0 ) {
      fclose( f );
      return 0;
   }
   fclose( f );
   return 1;
}

unsigned char calc_chksum( int algo, unsigned char *data, size_t size, 
	                       unsigned short addr )
{
	unsigned char *endp = data + size;
	unsigned short chksum = 0;
	unsigned short carry = 0;

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

unsigned char calculate( int algo, unsigned char *data, size_t size,
	           unsigned short addr, unsigned short chksum_addr )
{
	unsigned char chksum;

	data[chksum_addr - addr] = '\0';
	chksum = ((addr >> 8) - calc_chksum( algo, data, size, addr )) & 0xff;
	
	printf( "check-sum byte at %04X = %02X\n", chksum_addr, chksum);
	return chksum;
}

int verify( int algo, unsigned char *data, size_t size, unsigned short addr )
{
	unsigned char correct_chksum = addr >> 8;
	unsigned char chksum;

	chksum = calc_chksum( algo, data, size, addr );
	
	printf( "data range %4hX-%4lX\n", addr, addr + size - 1);
	if ( chksum == correct_chksum ) {
		printf( "check-sum is correct: %2X\n", chksum );
	}
	else {
		printf( "CHECK-SUM MISMATCH: is %2X, should be %2X\n", chksum, correct_chksum);
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

	if ( !strcmp( argv[2], "old" ) ) algo = 1;
	else if ( !strcmp( argv[2], "new" ) ) algo = 2;
	else {
		printf( "error: invalid algorithm\n" );
		goto error;
	}

	if ( !hexstr_to_num(argv[4], &load_addr) ) {
		printf("error: illegal load address\n");
		goto error;
	}

	file_data = load_file( argv[3], &file_size );
	if ( !file_data ) {
		printf( "error: can not load input file\n");
		goto error;
	}

	if (  !strcmp( argv[1], "calculate") || !strcmp( argv[1], "patch") ) {		
		if ( argc < 6 ) goto usage;
		if ( !hexstr_to_num(argv[5], &chksum_addr) || 
			(chksum_addr < load_addr || chksum_addr > load_addr + file_size - 1 ) ) {
			printf("error: illegal check-sum address\n");
			free( file_data );
			goto error;
		}
	}

	if ( !strcmp(argv[1], "verify") ) {
		verify( algo, file_data, file_size, load_addr );
	}
	else if ( !strcmp( argv[1], "calculate") ) {
		calculate( algo, file_data, file_size, load_addr, chksum_addr );
	}
	else if (  !strcmp( argv[1], "patch") ) {
		if ( argc != 7 ) goto usage;
		file_data[chksum_addr - load_addr] =
			calculate( algo, file_data, file_size, load_addr, chksum_addr );
		if ( !write_file( argv[6], file_data, file_size ) ) {
			printf("error: can not write output file\n" );
		}
	}
	else {
		goto usage;
	}

	free( file_data );
	return EXIT_SUCCESS;

usage:
	print_usage();
error:
	if ( file_data ) free( file_data );
	return EXIT_FAILURE;
}
