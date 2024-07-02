/* 
 * File:	types.h
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#ifndef __TYPES_H
#define	__TYPES_H

#ifdef __cplusplus
extern "C" {
#endif



	typedef unsigned char 	uchar;
	typedef unsigned short 	ushort;
	typedef unsigned long 	ulong;
	
	#ifndef TRUE
		#define TRUE 	1
	#endif
	#ifndef FALSE
		#define FALSE 	0
	#endif
	#ifndef NULL
		#define NULL 	0
	#endif
	
	
	
	typedef struct {
		unsigned char* data;
		unsigned short size;
		unsigned short len;
	} line_buff_uchar_t;
	
	typedef struct {
		short* data;
		unsigned short size;
		unsigned short len;
	} line_buff_short_t;

	typedef struct {
		unsigned char* data;
		unsigned short size;
		unsigned short end; /// end to write, begin to read
		unsigned short len;
	} ring_buff_uchar_t;

	
	typedef struct {
		unsigned short x;
		unsigned short y;
	} pos_px_t;
	
	typedef struct {
		unsigned short w;
		unsigned short h;
	} rect_px_t;
	
	typedef struct {
		unsigned short columns;
		unsigned short rows;
	} table_t;
	
	typedef struct {
		unsigned short column;
		unsigned short row;
	} cell_t;

	
	typedef struct { // 1bit = 1pixel
		const unsigned short width;
		const unsigned short height;
		const unsigned char* data;
		const unsigned short size;
	} image_mono_const_uchar_t;
	
	
	
	// Array operations
	void array_uchar_clear( unsigned char null_byte, unsigned char* arr, unsigned short *arr_len );
	void array_uchar_fill( unsigned char fill_char, unsigned char* arr, unsigned short arr_len );
	void array_uchar_put( unsigned short put_arr_ind, unsigned char* put_arr, unsigned short put_arr_len, unsigned short arr_ind, unsigned char* arr, unsigned short *arr_len, unsigned short arr_size );
	void array_uchar_shift( short shift, unsigned short arr_ind, unsigned char null_byte, unsigned char* arr, unsigned short *arr_len, unsigned short arr_size ); // shift < 0 - left shift
	void array_uchar_rotare( short rotate, unsigned char* arr, unsigned short arr_len ); // rotate < 0 - left rotate
	/// array move and copy bytes

	// Ring buffer operations
	int  ring_buff_add( ring_buff_uchar_t src_buff, ring_buff_uchar_t *dest_buff );
	int  ring_buff_copy( ring_buff_uchar_t src_buff, ring_buff_uchar_t *dest_buff );
	int  ring_buff_move( ring_buff_uchar_t *src_buff, ring_buff_uchar_t *dest_buff );
	int  ring_buff_read_set( ushort read_ind, ring_buff_uchar_t *ring_buff );
	//void ring_buff_allign_left( ring_buff_uchar_t *ring_buff );
	//int  ring_to_line_buff( ring_buff_uchar_t *ring_buff, line_buff_uchar_t *line_buff );
	//
	int   ring_buff_add_string( uchar* string_, ushort string_len, ring_buff_uchar_t *ring_buff );
	int   ring_buff_add_short( short number, ring_buff_uchar_t *ring_buff );
	int   ring_buff_add_char( uchar char_, ring_buff_uchar_t *ring_buff );
	uchar ring_buff_found_char( uchar char_, ushort *found_index, ring_buff_uchar_t *ring_buff ); // TRUE or FALSE
	uchar ring_buff_found_string( uchar* string_, ushort string_len, ushort *found_index, ring_buff_uchar_t *ring_buff ); // TRUE or FALSE
	//
	int    ring_buff_end_set( ring_buff_uchar_t *ring_buff, ushort end_ind );
	ushort ring_buff_begin_get( ring_buff_uchar_t *ring_buff );
	ushort ring_buff_index_shift( ushort index, int shift, ushort size ); // shifted index
	void   ring_buff_reset( ring_buff_uchar_t *ring_buff );
	void   ring_buff_init( ring_buff_uchar_t *ring_buff, uchar null_byte );

	// Float operations
	float array_to_float( unsigned short arr_ind, unsigned char* arr );
	float array_to_float_rev( unsigned short arr_ind, unsigned char* arr );
	void float_to_array( float number, unsigned short arr_ind, unsigned char* arr );
	void float_to_array_rev( float number, unsigned short arr_ind, unsigned char* arr );
	unsigned char is_nan_float( float *value );
	long float_to_long( float value );

	// Ulong operations
	unsigned long array_to_ulong( unsigned short arr_ind, unsigned char* arr );
	unsigned long array_to_ulong_rev( unsigned short arr_ind, unsigned char* arr );
	void ulong_to_array( unsigned long number, unsigned short arr_ind, unsigned char* arr );
	void ulong_to_array_rev( unsigned long number, unsigned short arr_ind, unsigned char* arr );

	// Long operations
	long array_to_long( unsigned short arr_ind, unsigned char* arr );
	long array_to_long_rev( unsigned short arr_ind, unsigned char* arr );
	void long_to_array( long number, unsigned short arr_ind, unsigned char* arr );
	void long_to_array_rev( long number, unsigned short arr_ind, unsigned char* arr );

	// Ushort operations
	unsigned short array_to_ushort( unsigned short arr_ind, unsigned char* arr );
	unsigned short array_to_ushort_rev( unsigned short arr_ind, unsigned char* arr );
	void ushort_to_array( unsigned short number, unsigned short arr_ind, unsigned char* arr );
	void ushort_to_array_rev( unsigned short number, unsigned short arr_ind, unsigned char* arr );

	// Short operations
	short array_to_short( unsigned short arr_ind, unsigned char* arr );
	short array_to_short_rev( unsigned short arr_ind, unsigned char* arr );
	void short_to_array( short number, unsigned short arr_ind, unsigned char* arr );
	void short_to_array_rev( short number, unsigned short arr_ind, unsigned char* arr );

	// Double operations
	/// ToDo

	// Long long operations
	/// ToDo



#ifdef __cplusplus
}
#endif

#endif //.__TYPES_H
