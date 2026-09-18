/* 
 * File:	types.c
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#include "types.h"

#include <stdlib.h>
#include "functions.h"
#include "maths.h"
#include "strings.h"





//=============================================================================
// Ring buffer operations

void rbuff_init( ring_buffer_t *rbuff, uchar null_byte )
{
	if( rbuff->size < 1 ) return;
	for( unsigned short i = 0; i < rbuff->size; i++ ) rbuff->data[i] = null_byte;
	rbuff->read  = rbuff->size-1;
	rbuff->write = rbuff->size-1;
}

void rbuff_reset( ring_buffer_t *rbuff )
{
	if( rbuff->size < 1 ) return;
	rbuff->read  = rbuff->size-1;
	rbuff->write = rbuff->size-1;
}



int rbuff_copy_segment(ring_buffer_t* dest, ring_buffer_t* src, ushort begin, ushort end) 
{
    dest->read  = dest->size - 1;
    dest->write = dest->size - 1;
    
    ushort index = begin;
    while( index != rbuff_index_next(src, end) ) 
    {
        dest->write = rbuff_index_next( dest, dest->write );
        if (dest->write == dest->read) break;
        dest->data[dest->write] = src->data[ index ];
        index = rbuff_index_next( src, index );
    }
    
    return 0;
}



bool rbuff_compare_str( string_buffer_t str, ring_buffer_t *rbuff )
{
	if( str.len == 0 ) return false;
	if( rbuff->read == rbuff->write ) return false;

	ushort rbuff_size = rbuff->size;
	ushort rbuff_ind  = rbuff_begin_get( rbuff );
	for( ushort i = 0; i < str.len; i++ )
	{
		if( str.data[ i ] != rbuff->data[ rbuff_ind ] )
		{
			return false;
		}
		if( rbuff_ind++ >= rbuff_size ) rbuff_ind = 0;
	}
	
	return true;
}



bool rbuff_to_ushort( ring_buffer_t *rbuff,  ushort *ret_number )
{
	ushort size = rbuff->size; ushort len = rbuff_length_get( rbuff ); ushort write = rbuff->write; ushort read = rbuff->read; 
	ushort number = 0; ushort digit = 1; ushort numeral = 0; uchar tmp_uchar; const ushort digit_max = 10000; //ushort max value 65535
	while( write != read ) {
		tmp_uchar = rbuff->data[ write ];
		if( (tmp_uchar > 47) && (tmp_uchar < 58) ) { //ASCII codes for 0..9, 48 == '0' && 57 == '9'
			numeral = tmp_uchar - 48;
			if( (digit == digit_max) && (numeral > 6) && (number > 5535) ) return false;
			number += numeral * digit;
			if( digit == digit_max ) break; else digit *= 10;
		}
		else if( digit != 1 ) return false;		
		write = (write == 0) ? size : (write - 1);
	}
	*ret_number = number;	
	return true;
}

bool rbuff_to_ushort_hex( ring_buffer_t *rbuff,  ushort *ret_number )
{
	ushort size = rbuff->size; ushort len = rbuff_length_get( rbuff ); ushort write = rbuff->write; ushort read = rbuff->read; 
	//if(  (rbuff->data[ read ] == '0') && (rbuff->data[ ((read + 1) >= size) ? 0 : (read + 1) ] == 'x')  ){ read = ((read + 2) >= size) ? (read + 2 - size) : (read + 2); } // "0x" tolerance
	unsigned short number = 0x00; uchar tmp_uchar; unsigned char digit_hex = 0; // 1 digit_hex = 4bits
	while( (write != read) && (digit_hex < 4) ) {
		tmp_uchar = rbuff->data[ write ];
		if( (tmp_uchar >= '0') && (tmp_uchar <= '9') ){ tmp_uchar = tmp_uchar - '0'; }
	    else if( (tmp_uchar >= 'A') && (tmp_uchar <= 'F') ){ tmp_uchar = tmp_uchar - 'A' + 0x0A; } else if( (tmp_uchar >= 'a') && (tmp_uchar <= 'f') ){ tmp_uchar = tmp_uchar - 'a' + 0x0A; }   // 30 - 0, 39 - 9, 41 - A, 46 - F
		else if( (tmp_uchar == 'x') && (rbuff->data[ (write == 0) ? size : (write - 1) ] == '0') ){ break; } else return false;
		number += tmp_uchar << 4*digit_hex;
		digit_hex++;
		write = (write == 0) ? size : (write - 1);
	}
	*ret_number = number;
	return true;
}



bool rbuff_append_char( uchar wr_char, ring_buffer_t *rbuff )
{
	rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
	rbuff->data[ rbuff->write ] = wr_char;

	return true;
}

bool rbuff_append_text( const uchar *wr_text, ring_buffer_t *rbuff )
{
	if( wr_text[0] ) 
	{ 
		ushort text_len = 0; 
		while(  wr_text[ text_len ]  ) 
		{
			rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
			rbuff->data[ rbuff->write ] = wr_text[ text_len ];

			text_len++; 
		}
	}
	
	return true;
}
	
bool rbuff_append_ushort( ushort wr_ushort, ring_buffer_t *rbuff )
{
	rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
	rbuff->data[ rbuff->write ] = ( wr_ushort & 0xFF );
	rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
	rbuff->data[ rbuff->write ] = ( (wr_ushort >> 8) & 0xFF );

	return true;
}

bool rbuff_append_byte( uchar wr_byte, ring_buffer_t *rbuff )
{
	rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
	rbuff->data[ rbuff->write ] = wr_byte;

	return true;
}
	
bool rbuff_append_bytes( uchar *wr_bytes, ushort wr_len, ring_buffer_t *rbuff )
{
	ushort wrote_len = 0; 
	while(  wrote_len < wr_len  ) 
	{
		rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
		rbuff->data[ rbuff->write ] = wr_bytes[ wrote_len ];

		wrote_len++;
	}

	return true;
}

bool rbuff_append_rbuff( ring_buffer_t *src_rbuff, ring_buffer_t *dst_rbuff )
{
	ushort write_len 	= rbuff_length_get( src_rbuff );
	if( write_len == 0 ) return true;
	if( write_len >= dst_rbuff->size ) return false;	
	ushort dst_free_len = dst_rbuff->size - rbuff_length_get( dst_rbuff );
	if( write_len >= dst_free_len ) return false;
	
	ushort src_read = src_rbuff->read;
	ushort wrote_len = 0; 
	while(  wrote_len < write_len  ) 
	{
		dst_rbuff->write = ( (dst_rbuff->write + 1) >= dst_rbuff->size ) ? 0 : dst_rbuff->write + 1; 
		src_read = ( (src_read + 1) >= src_rbuff->size ) ? 0 : src_read + 1; 
		dst_rbuff->data[ dst_rbuff->write ] = src_rbuff->data[ src_read ];
		
		wrote_len++;
	}
	
	return true;
}

bool rbuff_append_str( string_buffer_t wr_str, ring_buffer_t *rbuff )
{
	ushort wrote_len = 0; 
	while(  wrote_len < wr_str.len  ) 
	{
		rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
		rbuff->data[ rbuff->write ] = wr_str.data[ wrote_len ];

		wrote_len++;
	}
	
	return true;
}

bool rbuff_append_number_str( short wr_number, ring_buffer_t *rbuff )
{
	static uchar  tmp_buff_data[6];
	ushort 		  tmp_buff_len;
	tmp_buff_len = 0;
	for( uchar i = 0; i < 6; i++ ) tmp_buff_data[i] = 0;
	short_to_string( wr_number, tmp_buff_data, &tmp_buff_len );
	
	ushort wrote_len = 0; 
	while(  wrote_len < tmp_buff_len  ) 
	{
		rbuff->write = ( (rbuff->write + 1) >= rbuff->size ) ? 0 : rbuff->write + 1; 
		rbuff->data[ rbuff->write ] = tmp_buff_data[ wrote_len ];
		
		wrote_len++;
	}

	return true;
}

//.Ring buffer operations
//=============================================================================





//=============================================================================
// Array operations
void array_uchar_clear( unsigned char null_byte, unsigned char* arr, unsigned short *arr_len )
{
	for( unsigned short i = 0; i < *arr_len; i++ ) arr[i] = null_byte;
	*arr_len = 0;
}

void array_uchar_fill( unsigned char fill_char, unsigned char* arr, unsigned short arr_len )
{
	for( unsigned short i = 0; i < arr_len; i++ ) arr[i] = fill_char;
}

void array_uchar_put( unsigned short put_arr_ind, unsigned char* put_arr, unsigned short put_arr_len, unsigned short arr_ind, unsigned char* arr, unsigned short *arr_len, unsigned short arr_size )
{
	if( ((arr_ind + put_arr_len - put_arr_ind) > arr_size) ) return;
	
	for( unsigned short i = put_arr_ind; i < put_arr_len; i++ ){ arr[ i + arr_ind ] = put_arr[ i ]; }
	
	*arr_len = arr_ind + put_arr_len - put_arr_ind;
}

void array_uchar_shift( short shift, unsigned short arr_ind, unsigned char null_byte, unsigned char* arr, unsigned short *arr_len, unsigned short arr_size ) // shift < 0 - left shift
{
	if( (shift == 0) || (*arr_len == 0) || (arr_size == 0) || (arr_ind >= *arr_len) ) return;
	
	if( shift > 0 )
	{
		if( ((*arr_len - arr_ind) + shift) > arr_size ) return;
		
		for( unsigned short i = *arr_len; i > arr_ind; i-- ) { arr[ i-1 + shift  ] = arr[ i-1 ]; }
		for( unsigned short i = arr_ind; i < arr_ind + shift; i++ ) { arr[ i ] = null_byte; }
		
		*arr_len = *arr_len + shift;
	}
	else
	{
		if( ((-1*shift) > arr_size) || ((-1*shift) > *arr_len) || ((-1*shift) > arr_ind) || (arr_ind == 0) ) return;
		
		for( unsigned short i = 0; i < arr_ind + shift+1; i++ ) { arr[ i ] = arr[ i-shift ]; }
		for( unsigned short i = arr_ind + shift+1; i < arr_ind+1; i++ ) { arr[ i ] = null_byte; }
		
		if( arr_ind == (*arr_len-1) ) *arr_len = *arr_len + shift;
	}	
}
//.array_uchar_shift()

void array_uchar_rotare( short rotate, unsigned char* arr, unsigned short arr_len ) // rotate < 0 - left rotate
{
	if( (rotate == 0) || (arr_len == 0) || (arr_len == 1) ) return;
	short _rotate = rotate % arr_len;
	unsigned short abs_rotate = abs_short( rotate );
	if( _rotate == 0 ) return;

	unsigned char* temp_arr;
	temp_arr = malloc( abs_rotate * sizeof(unsigned char*) );
	if( !temp_arr ) return;
	
	if( _rotate > 0 ) // right rotate
	{
		for( unsigned short i = 1; i <= abs_rotate; i++ )
		{
			temp_arr[ abs_rotate - i ] = arr[ arr_len - i ];
		}
		for( unsigned short i = arr_len-1; i >= abs_rotate; i-- )
		{
			arr[ i ] = arr[ i - abs_rotate ];
		}
		for( unsigned short i = 0; i < abs_rotate; i++ )
		{
			arr[ i ] = temp_arr[ i ];
		}
	}
	else // left rotate
	{
		for( unsigned short i = 0; i < abs_rotate; i++ )
		{
			temp_arr[ i ] = arr[ i ];
		}
		for( unsigned short i = 0; i < (arr_len - abs_rotate); i++ )
		{
			arr[ i ] = arr[ i + abs_rotate ];
		}
		for( unsigned short i = 0; i < abs_rotate; i++ )
		{
			arr[ arr_len - abs_rotate + i ] = temp_arr[ i ];
		}
	}
	
	free( temp_arr );
}
//.array_uchar_rotare()

//.Array operations
//=============================================================================



//=============================================================================
// Float operations
typedef union 
{ 
	float number;  
	unsigned char bytes[4]; 
} floatunion_t; 
floatunion_t floatunion;

float array_to_float( unsigned short arr_ind, unsigned char* arr )
{
	floatunion.bytes[0] = arr[ arr_ind + 0 ];
	floatunion.bytes[1] = arr[ arr_ind + 1 ];
	floatunion.bytes[2] = arr[ arr_ind + 2 ];
	floatunion.bytes[3] = arr[ arr_ind + 3 ];
		
	return floatunion.number;
}

float array_to_float_rev( unsigned short arr_ind, unsigned char* arr )
{
	floatunion.bytes[0] = arr[ arr_ind + 3 ];
	floatunion.bytes[1] = arr[ arr_ind + 2 ];
	floatunion.bytes[2] = arr[ arr_ind + 1 ];
	floatunion.bytes[3] = arr[ arr_ind + 0 ];

	return floatunion.number;
}

void float_to_array( float number, unsigned short arr_ind, unsigned char* arr )
{
	floatunion.number = number;
	
	arr[ arr_ind + 0 ] = floatunion.bytes[0];
	arr[ arr_ind + 1 ] = floatunion.bytes[1];
	arr[ arr_ind + 2 ] = floatunion.bytes[2];
	arr[ arr_ind + 3 ] = floatunion.bytes[3];
}

void float_to_array_rev( float number, unsigned short arr_ind, unsigned char* arr )
{
	floatunion.number = number;
	
	arr[ arr_ind + 0 ] = floatunion.bytes[3];
	arr[ arr_ind + 1 ] = floatunion.bytes[2];
	arr[ arr_ind + 2 ] = floatunion.bytes[1];
	arr[ arr_ind + 3 ] = floatunion.bytes[0];
}

unsigned char is_nan_float( float *value )
{
	return ((((unsigned char*)value)[2] & 0x80)==0x80) 
		&& ((((unsigned char*)value)[3] & 0x7F)==0x7F);
}

long float_to_long( float value )
{
	long value_round = 0;
	value_round = (long) value;

	static const double fraction_up = 0.5555555;
	if(  ( ((double)value + fraction_up) - value_round ) > 1  ) //round up to 1
	{
		value_round++;
	}
	
	return value_round;
}
//.Float operations
//=============================================================================



//=============================================================================
// Ulong operations
typedef union 
{ 
	unsigned long number;  
	unsigned char bytes[4]; 
} ulongunion_t; 
ulongunion_t ulongunion;

unsigned long array_to_ulong( unsigned short arr_ind, unsigned char* arr )
{
	ulongunion.bytes[0] = arr[ arr_ind + 0 ];
	ulongunion.bytes[1] = arr[ arr_ind + 1 ];
	ulongunion.bytes[2] = arr[ arr_ind + 2 ];
	ulongunion.bytes[3] = arr[ arr_ind + 3 ];

	return ulongunion.number;
}

unsigned long array_to_ulong_rev( unsigned short arr_ind, unsigned char* arr )
{
	ulongunion.bytes[0] = arr[ arr_ind + 3 ];
	ulongunion.bytes[1] = arr[ arr_ind + 2 ];
	ulongunion.bytes[2] = arr[ arr_ind + 1 ];
	ulongunion.bytes[3] = arr[ arr_ind + 0 ];
		
	return ulongunion.number;
}

void ulong_to_array( unsigned long number, unsigned short arr_ind, unsigned char* arr )
{
	ulongunion.number = number;
	
	arr[ arr_ind + 0 ] = ulongunion.bytes[0];
	arr[ arr_ind + 1 ] = ulongunion.bytes[1];
	arr[ arr_ind + 2 ] = ulongunion.bytes[2];
	arr[ arr_ind + 3 ] = ulongunion.bytes[3];
}

void ulong_to_array_rev( unsigned long number, unsigned short arr_ind, unsigned char* arr )
{
	ulongunion.number = number;
	
	arr[ arr_ind + 0 ] = ulongunion.bytes[3];
	arr[ arr_ind + 1 ] = ulongunion.bytes[2];
	arr[ arr_ind + 2 ] = ulongunion.bytes[1];
	arr[ arr_ind + 3 ] = ulongunion.bytes[0];
}
//.Ulong operations
//=============================================================================

//=============================================================================
// Long operations
typedef union 
{ 
	long number;  
	unsigned char bytes[4]; 
} longunion_t; 
longunion_t longunion;

long array_to_long( unsigned short arr_ind, unsigned char* arr )
{
	longunion.bytes[0] = arr[ arr_ind + 0 ];
	longunion.bytes[1] = arr[ arr_ind + 1 ];
	longunion.bytes[2] = arr[ arr_ind + 2 ];
	longunion.bytes[3] = arr[ arr_ind + 3 ];
		
	return longunion.number;
}

long array_to_long_rev( unsigned short arr_ind, unsigned char* arr )
{
	longunion.bytes[0] = arr[ arr_ind + 3 ];
	longunion.bytes[1] = arr[ arr_ind + 2 ];
	longunion.bytes[2] = arr[ arr_ind + 1 ];
	longunion.bytes[3] = arr[ arr_ind + 0 ];
		
	return longunion.number;
}

void long_to_array( long number, unsigned short arr_ind, unsigned char* arr )
{
	longunion.number = number;
	
	arr[ arr_ind + 0 ] = longunion.bytes[0];
	arr[ arr_ind + 1 ] = longunion.bytes[1];
	arr[ arr_ind + 2 ] = longunion.bytes[2];
	arr[ arr_ind + 3 ] = longunion.bytes[3];
}

void long_to_array_rev( long number, unsigned short arr_ind, unsigned char* arr )
{
	longunion.number = number;
	
	arr[ arr_ind + 0 ] = longunion.bytes[3];
	arr[ arr_ind + 1 ] = longunion.bytes[2];
	arr[ arr_ind + 2 ] = longunion.bytes[1];
	arr[ arr_ind + 3 ] = longunion.bytes[0];
}

//.Long operations
//=============================================================================



//=============================================================================
// Ushort operations
typedef union 
{ 
	unsigned short number;  
	unsigned char bytes[2]; 
} ushortunion_t; 
ushortunion_t ushortunion;

unsigned short array_to_ushort( unsigned short arr_ind, unsigned char* arr )
{
	ushortunion.bytes[0] = arr[ arr_ind + 0 ];
	ushortunion.bytes[1] = arr[ arr_ind + 1 ];
		
	return ushortunion.number;
}

unsigned short array_to_ushort_rev( unsigned short arr_ind, unsigned char* arr )
{
	ushortunion.bytes[0] = arr[ arr_ind + 1 ];
	ushortunion.bytes[1] = arr[ arr_ind + 0 ];
		
	return ushortunion.number;
}

void ushort_to_array( unsigned short number, unsigned short arr_ind, unsigned char* arr )
{
	ushortunion.number = number;
	
	arr[ arr_ind + 0 ] = ushortunion.bytes[0];
	arr[ arr_ind + 1 ] = ushortunion.bytes[1];
}

void ushort_to_array_rev( unsigned short number, unsigned short arr_ind, unsigned char* arr )
{
	ushortunion.number = number;
	
	arr[ arr_ind + 0 ] = ushortunion.bytes[1];
	arr[ arr_ind + 1 ] = ushortunion.bytes[0];
}
//.Ushort operations
//=============================================================================



//=============================================================================
// Short operations
typedef union 
{ 
	short number;  
	unsigned char bytes[2]; 
} shortunion_t; 
shortunion_t shortunion;

short array_to_short( unsigned short arr_ind, unsigned char* arr )
{
	shortunion.bytes[0] = arr[ arr_ind + 0 ];
	shortunion.bytes[1] = arr[ arr_ind + 1 ];
		
	return shortunion.number;
}

short array_to_short_rev( unsigned short arr_ind, unsigned char* arr )
{
	shortunion.bytes[0] = arr[ arr_ind + 1 ];
	shortunion.bytes[1] = arr[ arr_ind + 0 ];
		
	return shortunion.number;
}

void short_to_array( short number, unsigned short arr_ind, unsigned char* arr )
{
	shortunion.number = number;
	
	arr[ arr_ind + 0 ] = shortunion.bytes[0];
	arr[ arr_ind + 1 ] = shortunion.bytes[1];
}

void short_to_array_rev( short number, unsigned short arr_ind, unsigned char* arr )
{
	shortunion.number = number;
	
	arr[ arr_ind + 0 ] = shortunion.bytes[1];
	arr[ arr_ind + 1 ] = shortunion.bytes[0];
}
//.Short operations
//=============================================================================



//=============================================================================
// Double operations

//.Double operations
//=============================================================================



//=============================================================================
// Long long operations

//.Long long operations
//=============================================================================
