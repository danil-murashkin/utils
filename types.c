/* 
 * File:	types.c
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#include "types.h"

#include "stdlib.h" ///

#include "functions.h"
#include "maths.h"

#include "strings.h"





//=============================================================================
// Ring buffer operations
// Ring buffer operations
int  ring_buff_add( ring_buff_uchar_t src_buff, ring_buff_uchar_t *dest_buff )
{
	return 0;
}

int  ring_buff_copy( ring_buff_uchar_t src_buff, ring_buff_uchar_t *dest_buff )
{
	return 0;
}

int  ring_buff_move( ring_buff_uchar_t *src_buff, ring_buff_uchar_t *dest_buff )
{
	return 0;
}


int  ring_buff_read_set( ushort read_ind, ring_buff_uchar_t *ring_buff )
{

	/*if( (ring_buff->end + ring_buff->len) <= ring_buff->size )
	{
		if( read_ind > ring_buff->end ) error

	}


	if( ring_buff->end > ring_buff->len ) begin = ring_buff->size - ring_buff->end - ring_buff->len;



	rbuffer->len = ((rbuffer->end > packet.buff.end) ? 0 : rbuffer->size) + rbuffer->end - packet.buff.end;*/
	return ERROR__NONE;
}



int   ring_buff_add_string( uchar* string_, ushort string_len, ring_buff_uchar_t *ring_buff ) // ERROR CODE
{
	ushort shift;
	string_insert_string( string_, string_len, ring_buff->end+1, ring_buff->data, &shift, ring_buff->size );
	ring_buff_end_set( ring_buff, ring_buff->end + string_len );
	return ERROR__NONE;
}

int   ring_buff_add_short( short number, ring_buff_uchar_t *ring_buff ) // ERROR CODE
{
	ushort shift;
	short_to_string( number, ring_buff->data + ring_buff->end+1, &shift );
	ring_buff_end_set( ring_buff, ring_buff->end + shift );
	return ERROR__NONE;
}

int   ring_buff_add_char( uchar char_, ring_buff_uchar_t *ring_buff ) // ERROR CODE
{
	ushort shift;
	string_insert_char( char_, ring_buff->end+1, ring_buff->data, &shift, ring_buff->size );
	ring_buff_end_set( ring_buff, ring_buff->end + 1 );
	return ERROR__NONE;
}

uchar ring_buff_found_char( uchar char_, ushort *found_index, ring_buff_uchar_t *ring_buff ) // TRUE or FALSE
{
	return string_found_char( found_index, char_, ring_buff->data, ring_buff->len ); 
} 

uchar ring_buff_found_string( uchar* string_, ushort string_len, ushort *found_index, ring_buff_uchar_t *ring_buff ) // TRUE or FALSE
{
	return string_found_substring( found_index, string_, string_len, ring_buff->data, ring_buff->len );
}



int   ring_buff_end_set( ring_buff_uchar_t *ring_buff, ushort end_ind )
{
	if( end_ind > (ring_buff->size-1) )
	{
		return -2;
	}

	ushort shift = end_ind + ((end_ind < ring_buff->end) ? ring_buff->size : 0) - ring_buff->end  + ((ring_buff->len == 0) ? 1 : 0);
		
	if( (shift + ring_buff->len) > ring_buff->size )
	{
		return -1; // overflow
	}

	ring_buff->len += shift;
	ring_buff->end = end_ind;

	return ERROR__NONE;
}
//.ring_buff_end_set()

ushort ring_buff_begin_get( ring_buff_uchar_t *ring_buff )
{
	if( ring_buff->len == 0 )
	{
		return ring_buff->end;
	}
	
	if( ring_buff->end < (ring_buff->len-1) )
	{
		return ring_buff->size +  ring_buff->end - (ring_buff->len-1);
	}
	else
	{
		return ring_buff->end - (ring_buff->len-1);
	}
}
//.ring_buff_begin_get()

ushort ring_buff_index_shift( ushort index, int shift, ushort size ) // shifted index
{
	int shift_ = shift % size;
	ushort ret_index = index;
	

	if( shift_ < 0 )
	{
		ret_index = index + (((index + shift_) < 0) ? size : 0) + shift_;
	}
	else
	{
		ret_index = index + shift_ - (((index + shift_) >= size) ? size : 0);	
	}

	return ret_index;
}
//.ring_buff_index_shift()



void   ring_buff_init( ring_buff_uchar_t *ring_buff, uchar null_byte )
{
	for( unsigned short i = 0; i < ring_buff->size; i++ ) ring_buff->data[i] = null_byte;
	ring_buff->len = 0;
	ring_buff->end = 0;
}

void   ring_buff_reset( ring_buff_uchar_t *ring_buff )
{
	ring_buff->len = 0;
	ring_buff->end = 0;
}



/*int cyclic_array_add_array( cyclic_array_uchar_t *cyclic_array, uchar* array, ushort array_len )
{
	if( (cyclic_array->begin >= cyclic_array->size) || (cyclic_array->end >= cyclic_array->size) )
	{
		/// error wrong cyclic_array pointers to end or begin of data in array
		return -4;
	}

	ushort cyclic_array_lenght = (  cyclic_array->end + ((cyclic_array->begin <= cyclic_array->end) ? 0 : cyclic_array->size)  ) - cyclic_array->begin;
	if( (cyclic_array_lenght + array_len) > cyclic_array->size )
	{
		/// error add oversize array
		return -3;
	}


	ushort cyclic_array_new_end = cyclic_array->end + array_len;
	if( cyclic_array_new_end >= cyclic_array->size ) cyclic_array_new_end -= cyclic_array->size;

	if( (cyclic_array->end + array_len) < cyclic_array->size )
	{		
		for( ushort i = 0; i < array_len; i++ )
		{
			cyclic_array->data[ cyclic_array->end + i ] = array[i];
		}
		cyclic_array->end = cyclic_array->end + array_len;
	}
	else
	{
		for( ushort i = 0; i < (cyclic_array->size - cyclic_array->end); i++ )
		{
			cyclic_array->data[ cyclic_array->end + i ] = array[i];
		}
		for( ushort i = (cyclic_array->size - cyclic_array->end); i < array_len; i++ )
		{
			cyclic_array->data[ i - (cyclic_array->size - cyclic_array->end) ] = array[i];
		}
		cyclic_array->end = cyclic_array->end - cyclic_array->size;
	}
	//.if( (cyclic_array->end + array_len) < cyclic_array->size )

	
	return ERROR__NONE;
}
//.cyclic_array_add_array()

int  cyclic_array_copy_array( cyclic_array_uchar_t cyclic_array,  ushort begin_offset, ushort copy_lenght,  uchar* destination_array, ushort destination_array_size )
{
	if( copy_lenght > destination_array_size )
	{
		/// error over write to array
		return -3;
	}

	if( (cyclic_array.begin >= cyclic_array.size) || (cyclic_array.end >= cyclic_array.size) )
	{
		/// error wrong cyclic_array pointers to end or begin of data in array
		return -2;
	}
	
	ushort cyclic_array_lenght = (  cyclic_array.end + ((cyclic_array.begin <= cyclic_array.end) ? 0 : cyclic_array.size)  ) - cyclic_array.begin;
	if( (begin_offset + copy_lenght) < cyclic_array_lenght )
	{
		/// error over shift + copy_lenght
		return -1;
	}


	ushort begin_to_size_len = cyclic_array.size - cyclic_array.begin;
	ushort copy_begin =	(begin_offset < begin_to_size_len) ? (cyclic_array.begin + begin_offset) : (begin_offset - begin_to_size_len);
	ushort copy_end   = ((begin_offset + copy_lenght) < begin_to_size_len) ? (cyclic_array.begin + (begin_offset + copy_lenght)) : ((begin_offset + copy_lenght) - begin_to_size_len);

	if( copy_begin <= copy_end )
	{		
		for( ushort i = 0; i < copy_lenght; i++ )
		{
			destination_array[i] = cyclic_array.data[ copy_begin + i ];
		}
		for( ushort i = copy_lenght; i < destination_array_size; i++ )
		{
			destination_array[i] = 0x00;
		}		
	}
	else
	{
		for( ushort i = 0; i < begin_to_size_len; i++ )
		{
			destination_array[i] = cyclic_array.data[ copy_begin + i ];
		}
		for( ushort i = 0; i < copy_end; i++ )
		{
			destination_array[ i + begin_to_size_len ] = cyclic_array.data[i];
		}
		for( ushort i = copy_end + begin_to_size_len; i < destination_array_size; i++ )
		{
			destination_array[i] = 0x00;
		}		
	}
	//.if( copy_end <= copy_begin )
	
	
	return ERROR__NONE;
}
//.cyclic_array_copy_array()

int  cyclic_array_begin_shift( cyclic_array_uchar_t *cyclic_array, ushort begin_shift )
{
	if( begin_shift == 0 ) return ERROR__NONE;
	
	if( (cyclic_array->begin >= cyclic_array->size) || (cyclic_array->end >= cyclic_array->size) )
	{
		/// error wrong cyclic_array pointers to end or begin of data in array
		return -3;
	}

	ushort cyclic_array_lenght = (  cyclic_array->end + ((cyclic_array->begin <= cyclic_array->end) ? 0 : cyclic_array->size)  ) - cyclic_array->begin;
	if( begin_shift < cyclic_array_lenght )
	{
		/// error over begin_shift
		return -2;
	}
	
	ushort begin_to_size_len = cyclic_array->size - cyclic_array->begin;
	cyclic_array->begin = (begin_shift < begin_to_size_len) ? (cyclic_array->begin + begin_shift) : (begin_shift - begin_to_size_len);

	return ERROR__NONE;
}
//.cyclic_array_begin_shift()*/

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
