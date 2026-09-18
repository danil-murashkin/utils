// File:	protocoljson_data.c
// Author:  Danil Murashkin
// Created: 2024.01.18

#ifndef __PROTOCOLJSON_DATA_H
#define __PROTOCOLJSON_DATA_H

#ifdef __cplusplus
extern "C" {
#endif



	//{
	//	"req":"firmware_update",
	//	"chunk":{
	//		"id":0,
	//		"total":12,
	//		"len":@count bytes if data@,
	//		"data":"@Base64 bytes@",
	//		"hash":"@CRC-16/MODBUS of bytes in data@"
	//	}
	//}
	
	
	
	#include "ide_config.h"
	#include "mcu_config.h"
	#include "types.h"
	
	
	
	typedef enum { // protocoljson_stage_t
		protocoljson_stage__undefined = 0,
		protocoljson_stage__init,
		
		protocoljson_stage__packet_waiting,
		protocoljson_stage__packet_receiving,
		protocoljson_stage__packet_received,
		
		protocoljson_stage__command_execute,
		protocoljson_stage__response_send
	} protocoljson_stage_t;
	
	static const ushort 	protocoljson_stage__timeouts_ms[] = {
		// 0 - measns endless
		0, 	// protocoljson_stage__undefined 
		0, 	// protocoljson_stage__init,
		
		0, 	// protocoljson_stage__packet_waiting
		300,// protocoljson_stage__packet_receiving,
		0,  // protocoljson_stage__packet_received,
		
		900,// protocoljson_stage__command_execute,
		100 // protocoljson_stage__response_send
	};
	
	
	typedef enum { // protocoljson_result_t
		protocoljson_result__undefined = 0,
		protocoljson_result__processing,
		protocoljson_result__success,
		
		protocoljson_result__error_preambula,
		protocoljson_result__error_json_not_closed,
		protocoljson_result__error_undefined_command,
		protocoljson_result__error_data_structure_mistake,
		protocoljson_result__error_command_execute
	} protocoljson_result_t;
	
	static const int 		protocoljson__error_none	= 0;
		// -100..-130 protocoljson_parameter_get errors
	
	static const ushort 	protocoljson__package_size_min = 			10;
	static const ushort 	protocoljson__preambula_tryout = 			20;
	
	
	typedef enum { // protocoljson_method_t
		protocoljson_method__undefined = 0,
		protocoljson_method__request,
		protocoljson_method__response
	} protocoljson_method_t;
	static const string_buffer_t protocoljson_method__names[] = {
		{ .data = "",    .len = 0 },
		{ .data = "req", .len = sizeof("req")-1 },
		{ .data = "res", .len = sizeof("res")-1  }
	};
	static const uchar protocoljson_method__count = sizeof( protocoljson_method__names ) / sizeof( string_buffer_t );
	
	static const string_buffer_t protocoljson_command__result = { .data = "result", .len = sizeof("result")-1 };
	
	
	
	typedef enum { 
		protocoljson_value_type__undefined = 0,
		protocoljson_value_type__number,
		protocoljson_value_type__string,
		protocoljson_value_type__array,
		protocoljson_value_type__structure
	} protocoljson_values_type_t;
	
	typedef struct { 
		protocoljson_values_type_t 	type;
		ring_buffer_t				name;
		ring_buffer_t				value;
	} protocoljson_parameter_t;
	
	
	// "chunk":{ "id":0, "total":12, "len":2048, "data":"@Base64 bytes@", "hash":"@CRC-16/MODBUS of bytes in data@" }
	static const string_buffer_t protocoljson_chunk__chunk_name = { .data = "chunk", .len = sizeof("chunk")-1 };
	typedef struct { // protocoljson_chunk_t  
		ushort id;
		ushort total;		
		ushort len;		
		uchar  hash_f; 
		ushort hash; // @CRC-16/MODBUS
		protocoljson_values_type_t data_type;
		ring_buffer_t data;
	} protocoljson_chunk_t;
	typedef enum { 
		protocoljson_chunk__name_id = 0,
		protocoljson_chunk__name_total,
		protocoljson_chunk__name_len,
		protocoljson_chunk__name_data,
		protocoljson_chunk__name_hash
	} protocoljson_chunk__names_list;
	static const string_buffer_t protocoljson_chunk__names[] = {
		{ .data = "id",    .len = sizeof("id")-1 },
		{ .data = "total", .len = sizeof("total")-1  },
		{ .data = "len",   .len = sizeof("len")-1  },
		{ .data = "data",  .len = sizeof("data")-1  },
		{ .data = "hash",  .len = sizeof("hash")-1  }
	};
	static const uchar protocoljson_chunk__names_len = sizeof( protocoljson_chunk__names ) / sizeof( string_buffer_t );
	
	
	typedef struct { // protocoljson_t
		protocoljson_stage_t 	stage;
		protocoljson_result_t 	result;
		
		protocoljson_method_t 	method;
		uchar 					command_ind;
		int 					command_result_code;
		const string_buffer_t 	*command__names;
		uchar 					command__count;
		
		bool 					chunk_f;
		protocoljson_chunk_t 	chunk;
		
		ring_buffer_t			buffer;
		ushort   				preambula_try_counter;
	 	ushort 					packet_brackets_count;
	} protocoljson_t;



#ifdef  __cplusplus
}
#endif

#endif //.__PROTOCOLJSON_DATA_H
