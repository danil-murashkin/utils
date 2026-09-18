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
	
	
	
	#include <stdbool.h>
	
	
	
	#ifndef TRUE
		#define TRUE 	1
	#endif
	#ifndef FALSE
		#define FALSE 	0
	#endif
	#ifndef NULL
		#define NULL 	0
	#endif
	
	typedef unsigned char 	uchar;
	typedef unsigned short 	ushort;
	typedef unsigned long 	ulong;
	

	
	typedef struct {
		ushort len;
		ushort size;
		uchar* data;
	} bytes_buffer_t;

	
	typedef struct {
		ushort  len;
		ushort  size;
		ushort* data;
	} ushort_buffer_t;

	typedef struct {
		ushort len;
		short  size;
		short* data;
	} short_buffer_t;
	
	typedef struct {
		ushort len;
		uchar* data;
	} string_buffer_t;

	typedef struct {
		ushort x;
		ushort y;
	} pos_px_t;
	
	typedef struct {
		ushort w;
		ushort h;
	} rect_px_t;
	
	typedef struct {
		ushort columns;
		ushort rows;
	} table_t;
	
	typedef struct {
		ushort column;
		ushort row;
	} cell_t;

	typedef struct { // 1bit = 1pixel
		const ushort width;
		const ushort height;
		const uchar* data;
		const ushort size;
	} image_mono_const_uchar_t;
	
	
	
	// Array operations
	void array_uchar_clear( uchar null_byte, uchar* arr, ushort *arr_len );
	void array_uchar_fill( uchar fill_char, uchar* arr, ushort arr_len );
	void array_uchar_put( ushort put_arr_ind, uchar* put_arr, ushort put_arr_len, ushort arr_ind, uchar* arr, ushort *arr_len, ushort arr_size );
	void array_uchar_shift( short shift, ushort arr_ind, uchar null_byte, uchar* arr, ushort *arr_len, ushort arr_size ); // shift < 0 - left shift
	void array_uchar_rotare( short rotate, uchar* arr, ushort arr_len ); // rotate < 0 - left rotate
	/// array move and copy bytes
	
	
	// Ring buffer operations
	typedef struct {
		ushort read;
		ushort write;
		ushort size;
		uchar* data;	
	} ring_buffer_t;

	void 	rbuff_init(  ring_buffer_t *rbuff, uchar null_byte );
	void 	rbuff_reset( ring_buffer_t *rbuff );
	static inline ushort rbuff_begin_get( ring_buffer_t *rbuff ) { return (rbuff->read == rbuff->write) ? rbuff->write : ((rbuff->read + 1) % rbuff->size); };
	static inline ushort rbuff_index_next(  ring_buffer_t *rbuff, ushort index ) { return (index + 1) % rbuff->size; };
	static inline ushort rbuff_index_prev(  ring_buffer_t *rbuff, ushort index ) { return (index == 0) ? (rbuff->size - 1) : (index - 1); };
	static inline ushort rbuff_index_read_next(  ring_buffer_t *rbuff ) { return (rbuff->read + 1) % rbuff->size; };
	static inline ushort rbuff_index_read_prev(  ring_buffer_t *rbuff ) { return (rbuff->read == 0) ? (rbuff->size - 1) : (rbuff->read - 1); };
	static inline ushort rbuff_index_shift( ring_buffer_t *rbuff, ushort index, ushort shift ) { return ((shift) == 0 ? (index) : ((index) + (shift)) % (rbuff)->size); };
	static inline ushort rbuff_length_get( ring_buffer_t *rbuff ) { return (rbuff->write >= rbuff->read) ? (rbuff->write - rbuff->read) : (rbuff->write + rbuff->size - rbuff->read); };
	static inline ushort rbuff_index_length_get( ring_buffer_t *rbuff, ushort begin, ushort end ) { return (end >= begin) ? (end - begin) : (end + (rbuff)->size - begin); };
	
	static inline bool rbuff_read_next(uchar *restrict return_char, ring_buffer_t *restrict rbuff) {
	    if (rbuff->read == rbuff->write) return false;	
	    ushort next = rbuff->read + 1;
	    rbuff->read = (next >= rbuff->size) ? 0 : next;	
	    *return_char = rbuff->data[rbuff->read];
	    return true;
	}
	#define rbuff_read_next_(rbuff, ret)             \
    do {                                             \
        if ((rbuff)->read == (rbuff)->write) break;  \
        (rbuff)->read = ((rbuff)->read + 1) & ((rbuff)->size - 1); \
        *(ret) = (rbuff)->data[(rbuff)->read];       \
    } while (0)
	

	void 	rbuff_copy( ring_buffer_t* dest, ring_buffer_t* src, ushort begin, ushort end );
	
	bool 	rbuff_to_ushort( ring_buffer_t *rbuff,  ushort *ret_number );
	bool 	rbuff_to_ushort_hex( ring_buffer_t *rbuff,  ushort *ret_number );
	
	bool 	rbuff_append_char( uchar wr_char, ring_buffer_t *rbuff );
	bool 	rbuff_append_ushort( ushort wr_ushort, ring_buffer_t *rbuff );
	bool 	rbuff_append_text( const uchar *wr_text, ring_buffer_t *rbuff );	
	bool 	rbuff_append_byte( uchar wr_byte, ring_buffer_t *rbuff );
	bool 	rbuff_append_bytes( uchar *wr_bytes, ushort wr_len, ring_buffer_t *rbuff );
	bool 	rbuff_append_rbuff( ring_buffer_t *src_rbuff, ring_buffer_t *dst_rbuff );
	bool 	rbuff_append_str( string_buffer_t wr_str, ring_buffer_t *rbuff );
	bool 	rbuff_append_number_str( short wr_number, ring_buffer_t *rbuff );	
	
	bool 	rbuff_compare_str( string_buffer_t str, ring_buffer_t *rbuff );
	
	
	// Float operations
	float  array_to_float( ushort arr_ind, uchar* arr );
	float  array_to_float_rev( ushort arr_ind, uchar* arr );
	void   float_to_array( float number, ushort arr_ind, uchar* arr );
	void   float_to_array_rev( float number, ushort arr_ind, uchar* arr );
	uchar  is_nan_float( float *value );
	long   float_to_long( float value );

	// Ulong operations
	ulong  array_to_ulong( ushort arr_ind, uchar* arr );
	ulong  array_to_ulong_rev( ushort arr_ind, uchar* arr );
	void   ulong_to_array( ulong number, ushort arr_ind, uchar* arr );
	void   ulong_to_array_rev( ulong number, ushort arr_ind, uchar* arr );

	// Long operations
	long   array_to_long( ushort arr_ind, uchar* arr );
	long   array_to_long_rev( ushort arr_ind, uchar* arr );
	void   long_to_array( long number, ushort arr_ind, uchar* arr );
	void   long_to_array_rev( long number, ushort arr_ind, uchar* arr );

	// Ushort operations
	ushort array_to_ushort( ushort arr_ind, uchar* arr );
	ushort array_to_ushort_rev( ushort arr_ind, uchar* arr );
	void   ushort_to_array( ushort number, ushort arr_ind, uchar* arr );
	void   ushort_to_array_rev( ushort number, ushort arr_ind, uchar* arr );

	// Short operations
	short  array_to_short( ushort arr_ind, uchar* arr );
	short  array_to_short_rev( ushort arr_ind, uchar* arr );
	void   short_to_array( short number, ushort arr_ind, uchar* arr );
	void   short_to_array_rev( short number, ushort arr_ind, uchar* arr );

	// Double operations
	/// ToDo

	// Long long operations
	/// ToDo



#ifdef __cplusplus
}
#endif

#endif //.__TYPES_H
