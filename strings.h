/* 
 * File:	strings.h
 * Author:  Danil Murashkin
 * Created: 2020.06.25 04:03
 */

#ifndef __STRINGS_H
#define	__STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif



	#include "types.h"
	
	

	// String operations
	unsigned char string_found_substring( unsigned short *substring_index, unsigned char* substring, unsigned short substring_len, unsigned char* string_, unsigned short string_len ); // TRUE or FALSE
	unsigned char string_found_char( unsigned short *char_index, unsigned char char_, unsigned char* string_, unsigned short string_len ); // TRUE or FALSE
	unsigned char string_compare( unsigned char* first, unsigned char* second, unsigned short length ); // TRUE or FALSE
	void string_cut( unsigned short index, unsigned short cut_len, unsigned char* string_,  unsigned short *string_len );
	void string_shift( unsigned short index, short shift, unsigned char* string_, unsigned short *string_len, unsigned short string_size );
	void string_insert_string( unsigned char* insert_string, unsigned short insert_string_len, unsigned short insert_index, unsigned char* string_, unsigned short *string_len, unsigned short string_size );
	void string_insert_char( unsigned char insert_char, unsigned short insert_index, unsigned char* string_, unsigned short *string_len, unsigned short string_size );

	// Value convert to string
	/// ToDo void float_to_string( float number_in, unsigned char* string_out, unsigned short *string_out_len );
	void long_to_string( long number_in, unsigned char* string_out, unsigned short *string_out_len );
	void ulong_to_string( unsigned long number_in, unsigned char* string_out, unsigned short *string_out_len );
	void short_to_string( short number_in, unsigned char* string_out, unsigned short *string_out_len );
	void ushort_to_string( unsigned short number_in, unsigned char* string_out, unsigned short *string_out_len );
	void uchar_to_string( unsigned char number_in, unsigned char*string_out, unsigned short *string_out_len );

	// String convert to value
	unsigned char string_to_float( unsigned char* string_in, unsigned short string_in_len, float *number_out ); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_to_long( unsigned char* string_in, unsigned short string_in_len, long *number_out); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_to_ulong( unsigned char* string_in, unsigned short string_in_len, unsigned long *number_out ); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_to_short( unsigned char* string_in, unsigned short string_in_len, short *number_out); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_to_ushort( unsigned char* string_in, unsigned short string_in_len, unsigned short *number_out ); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_to_uchar( unsigned char* string_in, unsigned short string_in_len, unsigned char *number_out ); // return FUNC__DENY or FUNC__CONFIRM

	// Hex string convert to value
	/// tolerance to 0x...
	unsigned char string_hex_to_uchar( unsigned char* string_in, unsigned short string_in_len, unsigned char *number_out ); // return FUNC__DENY or FUNC__CONFIRM
	unsigned char string_hex_to_ushort( unsigned char* string_in, unsigned short string_in_len, unsigned short *number_out ); // return FUNC__DENY or FUNC__CONFIRM



#ifdef __cplusplus
}
#endif

#endif //.__STRINGS_H
