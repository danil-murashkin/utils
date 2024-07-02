// File:    base64.h
// Author:  Danil Murashkin
// Created: 2024.04.18

#ifndef __BASE64_H
#define __BASE64_H

#ifdef __cplusplus
extern "C" {
#endif


	
	// Theory: 	https://ilyachalov.livejournal.com/201366.html
	// Tool: 	https://base64.guru/converter/encode/hex
	//
	// Reference:
	// 		https://github.com/Alexpux/MSYS2-pacman/blob/master/lib/libalpm/base64.h
	// 		https://github.com/zhicheng/base64/tree/master
	
	#include "../utils/types.h"
	
	
	
	/// support splitted bytes /r/n /n ' ' = = = 
	/// support base64 for url * -
	/// calculate decoded lenght without =

    int  base64_decode( uchar* source_data, ushort source_len,  uchar* destination_data, ushort destination_size,  ushort* decoded_len );
	int  base64_decode_rbuff( ring_buff_uchar_t *source,  ring_buff_uchar_t *destination );
	
	/// int  base64_encode(  );
	/// int  base64_encode_rbuff( ring_buff_uchar_t *source,  ring_buff_uchar_t *destination );
	
	int  base64_encoded_buff_valid_check_rbuff( ring_buff_uchar_t *encoded_buff );
	int  base64_encoded_buff_valid_check( uchar* encoded_data, ushort encoded_len );
	ushort base64_decoded_len_get( ushort encoded_bytes_len ); // decoded_len
	ushort base64_encode_len_get( ushort decoden_bytes_len );  // encoded_len



#ifdef __cplusplus
}
#endif

#endif //.__BASE64_H
