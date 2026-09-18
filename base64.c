// File:    base64.c
// Author:  Danil Murashkin
// Created: 2024.04.18

#include "base64.h"

#include "functions.h"
#include <stdbool.h>
#include "types.h"



/*static const char base64_encode_map[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};*/

static const unsigned char base64_symbol_pad = '='; // =64 skiped bytes 
static const unsigned char base64_decode_map[] = {
	// nul, soh, stx, etx, eot, enq, ack, bel,
	   255, 255, 255, 255, 255, 255, 255, 255,
	//  bs,  ht,  nl,  vt,  np,  cr,  so,  si,
	   255, 255, 255, 255, 255, 255, 255, 255,
	// dle, dc1, dc2, dc3, dc4, nak, syn, etb,
	   255, 255, 255, 255, 255, 255, 255, 255,
	// can,  em, sub, esc,  fs,  gs,  rs,  us,
	   255, 255, 255, 255, 255, 255, 255, 255,
	//  sp, '!', '"', '#', '$', '%', '&', ''',
	   255, 255, 255, 255, 255, 255, 255, 255,
	// '(', ')', '*', '+', ',', '-', '.', '/',
	   255, 255, 255,  62, 255, 255, 255,  63,
	// '0', '1', '2', '3', '4', '5', '6', '7',
		52,  53,  54,  55,  56,  57,  58,  59,
	// '8', '9', ':', ';', '<', '=', '>', '?',
		60,  61, 255, 255, 255, 64,  255, 255,
	// '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	   255,   0,   1,  2,   3,   4,   5,    6,
	// 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
		 7,   8,   9,  10,  11,  12,  13,  14,
	// 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
		15,  16,  17,  18,  19,  20,  21,  22,
	// 'X', 'Y', 'Z', '[', '\', ']', '^', '_',
		23,  24,  25, 255, 255, 255, 255, 255,
	// '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	   255,  26,  27,  28,  29,  30,  31,  32,
	// 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
		33,  34,  35,  36,  37,  38,  39,  40,
	// 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
		41,  42,  43,  44,  45,  46,  47,  48,
	// 'x', 'y', 'z', '{', '|', '}', '~', del,
		49,  50,  51, 255, 255, 255, 255, 255
};





int  base64_decode( uchar* source_data, ushort source_len,  uchar* destination_data, ushort destination_size,  ushort* decoded_len )
{
	*decoded_len = 0;
	
	if( source_len == 0 )
	{
		return ERROR__NONE;
	}
	if( base64_decoded_len_max_get( source_len ) > destination_size)
	{
		// not enought space in destination
		return -1; 
	}
	if( source_len & 0x3 )
	{
		// Invalid lenght of encoded data. Lenght must divide to 4
		return -2;
	}
	
	if( base64_encoded_buff_valid_check( source_data, source_len ) != ERROR__NONE )
	{
		// Invalid character in base64
		return -3;
	}
	
	
	bool  found_pad_at_next_f = false;
	
	uchar  encoded_char;
	ushort encoded_len = source_len;
	
    uchar  decoded_char;
    ushort decoded_bytes = 0;	
	for( ushort i = 0; i < encoded_len; i++ ) 
    { 
		encoded_char = source_data[i];
		decoded_char = base64_decode_map[ encoded_char ];		
		found_pad_at_next_f = (((encoded_len - i) < 4) && ((encoded_len - i) > 1) && (source_data[i+1] == base64_symbol_pad))  ? true : false;
		
		switch( i & 0x3 ) 
		{
			case 0:
				destination_data[ decoded_bytes ]    = (decoded_char << 2) & 0xFF;
			break;
				
			case 1:
				destination_data[ decoded_bytes ] |= (decoded_char >> 4) & 0x3;
				if( !found_pad_at_next_f )
				{
					decoded_bytes++;
					destination_data[ decoded_bytes ]    = (decoded_char & 0xF) << 4;
				}
			break;
				
			case 2:
				destination_data[ decoded_bytes ] |= (decoded_char >> 2) & 0xF;
				if( !found_pad_at_next_f )
				{
					decoded_bytes++;
					destination_data[ decoded_bytes ]    = (decoded_char & 0x3) << 6;
				}
			break;
				
			case 3:
				destination_data[ decoded_bytes ] |= decoded_char;
				if( !found_pad_at_next_f )
				{
					decoded_bytes++;
				}
			break;
		}
		
		if( found_pad_at_next_f )
		{
			break;
		}
	}
	//.for( ushort i = 0; i < encoded_len; i++ ) 
	
	
	*decoded_len = decoded_bytes + (  ( (decoded_len > 0) && found_pad_at_next_f ) ? 1 : 0  );
	
	return ERROR__NONE;
}
//.base64_decode()

int  base64_decode_rbuff( ring_buffer_t *source,  ring_buffer_t *destination )
{
	if( source->write == source->read )
	{
		// Invalid input size data
		return -1;
	}
	

	ushort source_len      = rbuff_length_get( source );
	ushort destination_len = rbuff_length_get( destination );

	if( (base64_decoded_len_max_get( source_len ) > (destination->size - destination_len)) 
	   && (source != destination) )
	{
		// not enought space in destination
		return -2; 
	}
	
	if( source_len & 0x3 )
	{
		// Invalid lenght of encoded data. Lenght must divide to 4
		return -3;
	}
	
	if( base64_encoded_buff_valid_check_rbuff( source ) != ERROR__NONE )
	{
		// Invalid character in base64
		return -4;
	}

	
	bool  found_pad_at_next_f = false;
	
    uchar  decoded_char;
	ushort decoded_size = destination->size;
    ushort decoded_len  = 0;
	ushort decoded_i    = (source != destination) ? ((destination_len > 0) ? (rbuff_index_shift( destination, destination->write, +1 )) : destination->write) : rbuff_begin_get( destination );
	
	uchar  encoded_char;
	ushort encoded_size = source->size;
	ushort encoded_len  = source_len;
	ushort encoded_i    = rbuff_begin_get( source );

	for( ushort i = 0; i < encoded_len; i++ )
	{
		encoded_char = source->data[ encoded_i ];
		decoded_char = base64_decode_map[ encoded_char ];
		found_pad_at_next_f = ((encoded_len - i) < 4) && ((encoded_len - i) > 1) && (source->data[ ((encoded_i + 1) >= encoded_size) ? 0 : (encoded_i + 1) ] == base64_symbol_pad)  ? true : false;

		switch( i & 0x3 ) 
		{
			case 0:
				destination->data[ decoded_i ] = (decoded_char << 2) & 0xFF;
			break;
				
			case 1:
				destination->data[ decoded_i ] |= (decoded_char >> 4) & 0x3;
				if( !found_pad_at_next_f )
				{
					decoded_len++;
					decoded_i++;
					if( decoded_i >= decoded_size ) decoded_i = 0;			
					destination->data[ decoded_i ] = (decoded_char & 0xF) << 4;
				}
			break;
				
			case 2:
				destination->data[ decoded_i ] |= (decoded_char >> 2) & 0xF;
				if( !found_pad_at_next_f )
				{
					decoded_len++;
					decoded_i++;
					if( decoded_i >= decoded_size ) decoded_i = 0;
					destination->data[ decoded_i ] = (decoded_char & 0x3) << 6;
				}
			break;
				
			case 3:
				destination->data[ decoded_i ] |= decoded_char;
				if( !found_pad_at_next_f )
				{
					decoded_len++;
					decoded_i++;
					if( decoded_i >= decoded_size ) decoded_i = 0;					
				}
			break;
		}
		
		if( found_pad_at_next_f )
		{
			break;
		}
		
		if( ++encoded_i >= encoded_size ) encoded_i = 0;
	}
	//.for( ushort i = 0; i < encoded_len; i++ )
	
	
	if( found_pad_at_next_f )
	{
		decoded_len += (decoded_len > 0) ? 1 : 0;
		destination->write = decoded_i;
	}
	else
	{
		destination->write = (decoded_i != 0) ? (decoded_i-1) : (decoded_size-1);
	}
		

	return ERROR__NONE;
}
//.base64_decode_rbuff()



int  base64_encode( unsigned char *dst, unsigned short *dlen,  unsigned char *src, unsigned short slen )
{
	return ERROR__NONE;
}
//.base64_encode()

int  base64_encode_rbuff( ring_buffer_t source,  ring_buffer_t *destination )
{
	return ERROR__NONE;
}
//.base64_encode_rbuff()



int  base64_encoded_buff_valid_check_rbuff( ring_buffer_t *encoded_buff )
{
    uchar encoded_char;
    uchar pad_counter = 0;
	
	ushort encoded_size = encoded_buff->size;
	ushort encoded_len  = rbuff_length_get( encoded_buff );
	ushort encoded_i    = rbuff_begin_get( encoded_buff );
	for( ushort i = 0; i < encoded_len; i++ )
	{
		encoded_char = encoded_buff->data[ encoded_i ];
		if( (encoded_char > 127) || (base64_decode_map[ encoded_char ] == 255) )
		{
			// Invalid character in base64
			return -1;
		}
		else if( encoded_char == base64_symbol_pad )
		{
			pad_counter++;
			if( pad_counter > 2 )
			{
				// Invalid character in base64
				return -1;
			}
		}
		else if( pad_counter > 0 )
		{
			// Invalid character in base64
			return -1;
		}
		

		if( ++encoded_i >= encoded_size ) encoded_i = 0;
	}


	return ERROR__NONE;
}
//.base64_encoded_buff_valid_check_rbuff()

int  base64_encoded_buff_valid_check( uchar* encoded_data, ushort encoded_len )
{
	uchar encoded_char;
	uchar pad_counter = 0;
	for( ushort i = 0; i < encoded_len; i++ )
	{
		encoded_char = encoded_data[i];
		if( (encoded_char > 127) || (base64_decode_map[ encoded_char ] == 255) )
		{
			// Invalid character in base64
			return -1;
		}
		else if( encoded_char == base64_symbol_pad )
		{
			pad_counter++;
			if( pad_counter > 2 )
			{
				// Invalid character in base64
				return -1;
			}
		}
		else if( pad_counter > 0 )
		{
			// Invalid character in base64
			return -1;
		}
	}

	return ERROR__NONE;
}
//.base64_encoded_buff_valid_check()

ushort base64_decoded_len_max_get( ushort encoded_bytes_len ) // decoded_len
{
	// 6 to 8 bits
	// v1
	// ulong  decoded_len = 0;
	// decoded_len = (encoded_bytes_len_max * 3) / 4;

	// v2
	ushort decoded_len_max = (encoded_bytes_len / 4) * 3;
	/// delete empty last bytes and rename to decoded_len

	return decoded_len_max;
}

ushort base64_encode_len_get( ushort decoden_bytes_len ) // encoded_len
{
	// 8 to 6 bits

	// v1
	// ulong encoded_len = 0;
	// ulong total_bits = decoden_bytes_len * 4;
	// encoded_len = total_bits / 3;
	// encoded_len += (total_bits > (encoded_len*3)) ? 1 : 0;

	// v2
	ushort encoded_len = ((decoden_bytes_len + 2) / 3) * 4 + 1;

	return encoded_len;
}
