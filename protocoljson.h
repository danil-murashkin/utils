// File:    protocoljson.h (PRoToCol based at JSON)
// Author:  Danil Murashkin
// Created: 2024.01.18

#ifndef __PROTOCOLJSON_H
#define __PROTOCOLJSON_H

#ifdef __cplusplus
extern "C" {
#endif



	#include "ide_config.h"
	#include "mcu_config.h"
	
	#include <stdbool.h>
	#include "types.h"
	
	#include "protocoljson_data.h"
	
	
	
	void protocoljson__update( ulong systick, ring_buffer_t *rx_buff,  protocoljson_t *protocol,
		bool (*command_execute_cb)(   protocoljson_t *protocol ), 
		bool (*response_send_cb)( protocoljson_t *protocol )
	);
	
	void protocoljson__response_generate( uchar command_ind, const string_buffer_t* command__names, int command_result_code, 
		bool chunk_resp, ushort chunk_id, 
		uchar* data_buff, ushort data_len, const uchar* text, 
		ring_buffer_t *response 
	);

	int   protocoljson_chunk_get( ring_buffer_t* buffer,  protocoljson_chunk_t *ret_chunk );
	int   protocoljson_parameter_get( ring_buffer_t *buffer,  protocoljson_parameter_t *parameter );
	bool  protocoljson_skip_json_empty_symbols( ring_buffer_t *rbuff );
	
	bool  protocoljson_send_packet_get( ring_buffer_t *send_buffer, ring_buffer_t *sending_packet );
	bool  protocoljson_send_packet_add( ring_buffer_t *data_buffer, ring_buffer_t *send_buffer );



#ifdef  __cplusplus
}
#endif

#endif //.__PROTOCOLJSON_H
