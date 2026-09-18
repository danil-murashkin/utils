// File:    protocoljson.c
// Author:  Danil Murashkin
// Created: 2024.01.18

#include "protocoljson.h"
#include "protocoljson_data.h"

#include "../utils/types.h"
#include "../utils/functions.h"





// protocoljson private
void protocoljson__stage_timeout( protocoljson_t *protocol );
void protocoljson__timeout_update( ulong systick, protocoljson_t *protocol, void(*stage_timeout_cb)(protocoljson_t *protocol) );
void protocoljson_command_get( protocoljson_t *protocol );
//.protocoljson private




//=============================================================================
// protocoljson public block

void protocoljson__update( ulong systick, ring_buffer_t *rx_buff,  protocoljson_t *protocol,
	bool (*command_execute_cb)(   protocoljson_t *protocol ), 
	bool (*response_send_cb)( protocoljson_t *protocol )
) 
{
	uchar tmp_char = 0x00;

	switch( protocol->stage )
	{
		case protocoljson_stage__init:
			protocol->result = protocoljson_result__undefined;
			protocol->command_result_code = protocoljson__error_none;
			protocol->preambula_try_counter = 0;
			protocol->stage = protocoljson_stage__packet_waiting;

		case protocoljson_stage__packet_waiting:
			while(  rbuff_read_next( &tmp_char, rx_buff )  )
			{
				if( tmp_char == '{' )
				{
					protocol->result 		= protocoljson_result__processing;
					
					protocol->method     	= protocoljson_method__undefined;
					protocol->command_ind	= 0; // command__undefined
					protocol->chunk_f		= false;
					
					protocol->buffer.data  	= rx_buff->data;
					protocol->buffer.size  	= rx_buff->size;
					protocol->buffer.read  	= rx_buff->read;
					protocol->buffer.write 	= 0;
					
					protocol->packet_brackets_count = 1;
					protocol->stage = protocoljson_stage__packet_receiving;
					break;
				}
				else
				{
					if( protocol->preambula_try_counter > protocoljson__preambula_tryout )
					{
						protocol->result = protocoljson_result__error_preambula;
						protocol->stage = protocoljson_stage__response_send;
					}
					else
					{
						protocol->preambula_try_counter++;
					}					
				}
			}
			break;

		case protocoljson_stage__packet_receiving:
			while(  rbuff_read_next( &tmp_char, rx_buff )  )
			{
				if( tmp_char == '{' ) protocol->packet_brackets_count++; 
				else if( tmp_char == '}' ) protocol->packet_brackets_count--; 
				protocol->buffer.write 	= rx_buff->read;
				if( protocol->packet_brackets_count == 0 )
				{
					protocol->stage = protocoljson_stage__packet_received;
				}
			}			
			break;

		
		case protocoljson_stage__packet_received:
			protocoljson_command_get( protocol );
			if( protocol->method == protocoljson_method__response ) 
			{
				protocol->stage = protocoljson_stage__init;
			}
			else if( (protocol->method != protocoljson_method__request) 
			      || (protocol->command_ind == 0)  )// 0 - command__undefined 
			{
				protocol->result = protocoljson_result__error_undefined_command;
				protocol->stage = protocoljson_stage__response_send;
			}
			else
			{
				protocol->stage = protocoljson_stage__command_execute;
			}
			break;
		

		case protocoljson_stage__command_execute:
			if(  command_execute_cb( protocol )  ) 
			{
				protocol->stage = protocoljson_stage__response_send;
			}
			break;

		case protocoljson_stage__response_send:
			if(  response_send_cb( protocol )  ) 
			{
				protocol->stage = protocoljson_stage__init;
			}
			break;

		default:
			protocol->stage = protocoljson_stage__init;
			break;
	} //.switch( protocol->stage )
	
	
	protocoljson__timeout_update( systick, protocol, protocoljson__stage_timeout );
}
//.protocoljson__update()



bool protocoljson_send_packet_add( ring_buffer_t *data_buffer, ring_buffer_t *send_buffer )
{
	ushort data_lenght = rbuff_length_get( data_buffer );
	
	rbuff_append_ushort( data_lenght, send_buffer );
	return rbuff_append_rbuff( data_buffer, send_buffer );
}
//.protocoljson_send_packet_add()

bool protocoljson_send_packet_get( ring_buffer_t *send_buffer, ring_buffer_t *sending_packet )
{
	// two first bytes of packet is lenght of packet
	ushort send_buffer_len = rbuff_length_get( send_buffer );
	if( send_buffer_len <= 2 )
	{
		return false;
	}
	
	ushort send_buffer_begin = rbuff_begin_get( send_buffer );
	ushort send_packet_lenght  = send_buffer->data[ send_buffer_begin ] 
		| ((send_buffer->data[ rbuff_index_shift( send_buffer, send_buffer_begin, +1 ) ] << 8) & 0xFF00); 
	
	if( (send_buffer_len - 2) < send_packet_lenght )
	{
		return false;
	}
	
	sending_packet->data  = send_buffer->data;
	sending_packet->size  = send_buffer->size;
	sending_packet->read  = rbuff_index_shift( send_buffer, send_buffer_begin, +2-1 ); 
	sending_packet->write = rbuff_index_shift( send_buffer, send_buffer_begin, +2-1 + send_packet_lenght );

	send_buffer->read = (sending_packet->write != 0) ? (sending_packet->write) : (sending_packet->size-1);
	

	return true;
}
//.protocoljson_send_packet_get()



void protocoljson__response_generate( uchar command_ind, const string_buffer_t* command__names, int command_result_code, 
	bool chunk_resp, ushort chunk_id, 
	uchar* data_buff, ushort data_len, const uchar* text, 
	ring_buffer_t *response )
{
	/// generate answer from command execution and delete bool chunk_resp, ushort chunk_id
	// {"res":"@command_chunk@","result":"1","chunk":{"id":0},"error":"@error_message@"}
	
	// reserve two first bytes for lenght of packet
	rbuff_append_byte( 0, response );
	ushort response_len_data_pointer = response->write;
	rbuff_append_byte( 0, response );
	

	rbuff_append_char( '{', response );

		// "res":"@command@"
		rbuff_append_char( '"', response );
		rbuff_append_str( protocoljson_method__names[ protocoljson_method__response ] , response );
		rbuff_append_char( '"', response );
		rbuff_append_char( ':', response );
		rbuff_append_char( '"', response );
		rbuff_append_str( command__names[ command_ind ] , response );
		rbuff_append_char( '"', response );
		
		// ,"result":"0"
		rbuff_append_char( ',', response );
		rbuff_append_char( '"', response );
		rbuff_append_str( protocoljson_command__result, response );
		rbuff_append_char( '"', response );
		rbuff_append_char( ':', response );
		rbuff_append_char( '"', response );
		rbuff_append_number_str( command_result_code, response );
		rbuff_append_char( '"', response );

		// ,"chunk":{"id":0}
		if( chunk_resp )
		{
			rbuff_append_char( ',', response );
			rbuff_append_bytes( protocoljson_chunk__chunk_name.data, protocoljson_chunk__chunk_name.len, response );
			rbuff_append_char( ':', response );
				rbuff_append_char( '{', response );
					string_buffer_t tmp_str = protocoljson_chunk__names[ protocoljson_chunk__name_id ];
					rbuff_append_bytes( tmp_str.data, tmp_str.len, response );
					rbuff_append_char( ':', response );
					rbuff_append_number_str( chunk_id, response );
				rbuff_append_char( '}', response );
		}

		// ,@data@
		if( data_len > 0 )
		{
			rbuff_append_char( ',', response );
			rbuff_append_bytes( data_buff, data_len, response );
		}

		// ,@text@
		if( text[0] ) 
		{
			rbuff_append_char( ',', response );
			rbuff_append_text( text, response );
		}

	rbuff_append_char( '}', response );


	// add ushort value of response_len to first bytes in ring_buffer_t
	ushort response_len = rbuff_index_length_get( response, response_len_data_pointer, response->write ) - 2;
	response->data[ response_len_data_pointer ] = response_len & 0x00FF;
	response->data[ rbuff_index_shift( response, response_len_data_pointer, +1 ) ] = (response_len >> 8) & 0x00FF;
}
//.protocoljson__response_generate()



int   protocoljson_chunk_get( ring_buffer_t* buffer,  protocoljson_chunk_t *ret_chunk )
{
	ring_buffer_t 			tmp_buffer = {
		.data   = buffer->data,
		.size   = buffer->size,
		.read   = buffer->read,
		.write  = buffer->write
	};
	protocoljson_parameter_t 	tmp_parameter;
	bool  parameter_chunk_founded = false;

	while( protocoljson_parameter_get( &tmp_buffer, &tmp_parameter ) == protocoljson__error_none )
	{
		if(  rbuff_compare_str( protocoljson_chunk__chunk_name, &tmp_parameter.name )  )
		{
			parameter_chunk_founded = true;
			break;
		}
	}
	

	if( !parameter_chunk_founded )
	{
		return -1;
	}
	else if( tmp_parameter.type != protocoljson_value_type__structure )
	{
		return -2;
	}
	
	
	tmp_buffer.data  = tmp_parameter.value.data;
	tmp_buffer.size  = tmp_parameter.value.size;
	tmp_buffer.read  = tmp_parameter.value.read;
	tmp_buffer.write = tmp_parameter.value.write;
	
	protocoljson_chunk_t tmp_chunk = { 
		.id     = 0, 
		.total  = 0, 
		.len    = 0, 
		.hash_f = false, 
		.hash   = 0,
		.data_type   = protocoljson_value_type__undefined,
		.data   = {
			.data   = tmp_parameter.value.data,
			.size   = tmp_parameter.value.size,
			.read   = tmp_parameter.value.size,
			.write  = tmp_parameter.value.size
		}  
	};
	bool chunk_id_f 	= false; 	bool chunk_id_value_f 	 = false;
	bool chunk_total_f 	= false; 	bool chunk_total_value_f = false;
	bool chunk_len_f 	= false; 	bool chunk_len_value_f 	 = false;
	bool chunk_data_f 	= false; 	bool chunk_data_value_f  = false;
	bool chunk_hash_f 	= false; 	bool chunk_hash_value_f  = false;

	while( protocoljson_parameter_get( &tmp_buffer, &tmp_parameter ) == protocoljson__error_none )
	{
		for( uchar i = 0; i < protocoljson_chunk__names_len; i++ )
		{
			if(  rbuff_compare_str( protocoljson_chunk__names[i], &tmp_parameter.name )  )
			{
				switch( (protocoljson_chunk__names_list)i )
				{

					case protocoljson_chunk__name_id:
					{
						chunk_id_f = true;
						
						if( tmp_parameter.type != protocoljson_value_type__number )
						{
							chunk_id_value_f = false;
						}
						else
						{
							ushort temp_number = 0;
							if( rbuff_to_ushort( &tmp_parameter.value, &temp_number ) == false )
							{
								chunk_id_value_f = false;
							}
							else
							{
								tmp_chunk.id = temp_number;
								chunk_id_value_f = true;
							}
						}
					}
						break;
						
						
					case protocoljson_chunk__name_total:
					{
						chunk_total_f = true;
						
						if( tmp_parameter.type != protocoljson_value_type__number )
						{
							chunk_total_value_f = false;
						}
						else
						{
							ushort temp_number = 0;
							if( rbuff_to_ushort( &tmp_parameter.value, &temp_number ) == false )
							{
								chunk_total_value_f = false;
							}
							else
							{
								tmp_chunk.total = temp_number;
								chunk_total_value_f = true;
							}
						}
					}
						break;
						
						
					case protocoljson_chunk__name_len: 
					{
						chunk_len_f = true;
						
						if( tmp_parameter.type != protocoljson_value_type__number )
						{
							chunk_len_value_f = false;
						}
						else
						{
							ushort temp_number = 0;
							if( rbuff_to_ushort( &tmp_parameter.value, &temp_number ) == false )
							{
								chunk_len_value_f = false;
							}
							else
							{
								tmp_chunk.len = temp_number;
								chunk_len_value_f = true;
							}
						}
					}
						break;
						
						
					case protocoljson_chunk__name_data:
					{
						chunk_data_f = true; 
						
						if( (tmp_parameter.type != protocoljson_value_type__string)
						 && (tmp_parameter.type != protocoljson_value_type__array) )
						{
							chunk_data_value_f = false;
						}
						else
						{
							chunk_data_value_f = true;
							tmp_chunk.data_type  = tmp_parameter.type;
							tmp_chunk.data.data  = tmp_parameter.value.data;
							tmp_chunk.data.size  = tmp_parameter.value.size;
							tmp_chunk.data.read  = tmp_parameter.value.read;
							tmp_chunk.data.write = tmp_parameter.value.write;
						}
					}
						break;
						
						
					case protocoljson_chunk__name_hash:
					{
						chunk_hash_f = true;
						
						if( (tmp_parameter.type != protocoljson_value_type__string) && (tmp_parameter.type != protocoljson_value_type__number) )
						{
							chunk_hash_value_f = false;
						}
						else 
						{
							ushort temp_number = 0;
							if( tmp_parameter.type == protocoljson_value_type__string )
							{
								if( rbuff_to_ushort_hex( &tmp_parameter.value, &temp_number ) == false )
								{
									chunk_hash_value_f = false;
								}
								else
								{
									tmp_chunk.hash_f = true;
									tmp_chunk.hash   = temp_number;
									chunk_hash_value_f = true;
								}
							}
							else if( tmp_parameter.type == protocoljson_value_type__number )
							{
								if( rbuff_to_ushort( &tmp_parameter.value, &temp_number ) == false )
								{
									chunk_hash_value_f = false;
								}
								else
								{
									tmp_chunk.hash_f = true;
									tmp_chunk.hash   = temp_number;
									chunk_hash_value_f = true;
								}
							}
						}
					}
						break;
						
						
				}
			} //.if(  rbuff_compare_str( protocoljson_chunk__names[i], tmp_parameter.name )  )
		} //.for( uchar i = 0; i < protocoljson_chunk__names_len; i++ )
	} //.while( protocoljson_parameter_get( &tmp_buffer, &tmp_parameter ) 

	if( (chunk_id_f  	== false) 	|| (chunk_id_value_f  	== false)
	 || (chunk_total_f 	== false) 	|| (chunk_total_value_f == false)
     || ((chunk_len_f 	== true)	&& (chunk_len_value_f 	== false))
	 ||	(chunk_data_f 	== false) 	|| (chunk_data_value_f  == false) 
	)
	{
		return -3;
	}
	
	///! delete
	if( (chunk_len_f == true) && (tmp_chunk.len == (rbuff_length_get(&tmp_chunk.data) + 2)) )
	{
		tmp_chunk.len -= 2;
	}
	
	if( (chunk_len_f == true) && (tmp_chunk.len != rbuff_length_get(&tmp_chunk.data)) )
	{
		return -4;
	}

	if( chunk_hash_f == true )
	{
		if( chunk_hash_value_f == false ) 
		{
			return -5;
		}
		else
		{
			if(0) ///( crc16_calc( tmp_chunk.data ) != chunk_hash_tmp )  
			{
				return -6;
			}
		}		
	}


	*ret_chunk = tmp_chunk;
	
	return protocoljson__error_none;
}
//.protocoljson__parse_chunk()



int   protocoljson_parameter_get( ring_buffer_t *buffer,  protocoljson_parameter_t *parameter )
{
	/// protocoljson_parameter_get update
	// . found to right ':'
	// . skip empty symbols before ':' and safe parameter name end 
	// . if parameter name end == '"' then shift symbol to left
	// . skip empty symbols to left
	// . found to left value from parameter name end to '"' or empty symbol or ',' '{' '}' '[' ']' 
	// . if parameter name > 1
	// . skip empty symbols after ':' and safe parameter value begin 
	// . if parameter value begin  == '"' then skip to right
	// . skip empty symbols to right
	// . found to right value from parameter name end to '"' or empty symbol or ',' '{' '}' '[' ']' 

	parameter->type = protocoljson_value_type__undefined;
	uchar tmp_char;
	
	
	// name of parameter
	if(  !protocoljson_skip_json_empty_symbols( buffer ) || !rbuff_read_next( &tmp_char, buffer )  )
	{
		return -101;
	}
	if( tmp_char == ',' )
	{
		if( !protocoljson_skip_json_empty_symbols( buffer ) || !rbuff_read_next( &tmp_char, buffer ) )
		{
			return -101;
		}
	}	
	if(   (tmp_char != '"')  ) 
	{
		return -101;
	} 
	
	parameter->name.data 	= buffer->data;
	parameter->name.size 	= buffer->size;
	parameter->name.read 	= buffer->read;
	parameter->name.write 	= parameter->name.read;
	
	bool parameter_name_end_founded = false;
	while(  rbuff_read_next( &tmp_char, buffer )  )
	{
		if( tmp_char == '"' )
		{
			parameter->name.write 	= rbuff_index_read_prev( buffer );
			
			parameter_name_end_founded = true;
			break;
		}
	}
	if( !parameter_name_end_founded )
	{
		return -102;
	}	
	//.name of parameter
	
	
	// separator between name and value
	if(  !protocoljson_skip_json_empty_symbols( buffer ) || !rbuff_read_next( &tmp_char, buffer ) 
		|| (tmp_char != ':')  )
	{
		return -103;
	}
	
	
	// value of parameter
	if(  !protocoljson_skip_json_empty_symbols( buffer ) || !rbuff_read_next( &tmp_char, buffer )  )
	{
		return -104;
	}
	
	parameter->value.data 	= buffer->data;
	parameter->value.size 	= buffer->size;
	parameter->value.read 	= buffer->read;
	parameter->value.write 	= buffer->read;

	switch( tmp_char )
	{
		// protocoljson_value_type__string
		case '"': {
			bool parameter_value_end_founded = false;
			while(  rbuff_read_next( &tmp_char, buffer )  )
			{
				if( tmp_char == ',' || tmp_char == '{' || tmp_char == '}' || tmp_char == '[' || tmp_char == ']')
				{
					return -121;
				}
				if( tmp_char == '"' )
				{
					parameter->value.read   = rbuff_begin_get( &parameter->value ); // skip " at begin
					parameter->value.write 	= rbuff_index_read_prev( buffer );		// skip " at end
					parameter->type 		= protocoljson_value_type__string;

					parameter_value_end_founded = true;
					break;
				}
			}
			if( !parameter_value_end_founded )
			{
				return -111;
			}
			} break;
		

		// protocoljson_value_type__array
		case '[': {
			bool parameter_value_end_founded = false;
			while(  rbuff_read_next( &tmp_char, buffer )  )
			{
				if( tmp_char == ']' )
				{
					parameter->value.read   = rbuff_begin_get( &parameter->value ); // skip [ at begin
					parameter->value.write 	= rbuff_index_read_prev( buffer );		// skip ] at end
					parameter->type 		= protocoljson_value_type__array;

					parameter_value_end_founded = true;
					break;
				}
			}
			if( !parameter_value_end_founded )
			{
				return -112;
			}
			} break;


		// protocoljson_value_type__structure
		case '{': {
			bool parameter_value_end_founded = false;
			int value_brackets_count = 1;
			while(  rbuff_read_next( &tmp_char, buffer )  )
			{
				if( tmp_char == '{' ) 		value_brackets_count++; 
				else if( tmp_char == '}' )  value_brackets_count--; 
				if( value_brackets_count == 0 )
				{
					parameter->value.write 	= buffer->read;
					parameter->type 		= protocoljson_value_type__structure;

					parameter_value_end_founded = true;
					break;
				}
			}
			if( !parameter_value_end_founded )
			{
				return -113;
			} 
			} break;


		// protocoljson_value_type__number
		case '.': // float value
		case '-': // belov zero
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
			bool float_pointer = (tmp_char == '.') ? true : false;
			bool parameter_value_end_founded = false;
			while(  rbuff_read_next( &tmp_char, buffer )  )
			{
				if( tmp_char == '.' ) 
				{
					if( float_pointer )
					{
						return -124;
					}
					float_pointer = true;
				}
				else if( (tmp_char < '0') || (tmp_char > '9') )
				{
					parameter->value.read   = rbuff_index_read_prev( &parameter->value );
					parameter->value.write 	= rbuff_index_read_prev( buffer );
					parameter->type 		= protocoljson_value_type__number;

					parameter_value_end_founded = true;
					break;
				}
			}
			if( !parameter_value_end_founded )
			{
				return -114;
			}
			} break;


		default:
			return -105;
	}
	//.value of parameter

	
	return protocoljson__error_none;
}
//.protocoljson_parameter_get()

bool  protocoljson_skip_json_empty_symbols( ring_buffer_t *rbuff )
{
	if( rbuff->read == rbuff->write ) return false;
	
	static const uchar jsons_skip_symbols[] = { ' ', '\n', '\r', '\0', '\t'};
	static const uchar jsons_skip_symbols_len = sizeof(jsons_skip_symbols);
	
	ushort i, read_ind;
	uchar  tmp_char;
	while( rbuff->read != rbuff->write )
	{
		read_ind = ((rbuff->read + 1) >= rbuff->size) ? 0 : (rbuff->read + 1);
		tmp_char = rbuff->data[ read_ind ];
		for( i = 0; i < jsons_skip_symbols_len; i++ )
		{
			if( tmp_char == jsons_skip_symbols[i] )
			{
				break;
			}
		}
		if( i == jsons_skip_symbols_len )
		{
			return true;
		}
		
		rbuff->read = read_ind;
	};

	return false;
}
//.protocoljson_skip_json_empty_symbols()

//.protocoljson public block
//=============================================================================





//=============================================================================
// protocoljson private

void protocoljson__stage_timeout( protocoljson_t *protocol )
{
	switch( protocol->stage )
	{
		case protocoljson_stage__packet_receiving:
			protocoljson_command_get( protocol );
			if( protocol->command_ind != 0 ) // 0 command__undefined
			{
				protocol->result = protocoljson_result__error_json_not_closed;
			}
			else if( protocol->method == protocoljson_method__request )
			{
				protocol->result = protocoljson_result__error_undefined_command;
			}
			else
			{
				protocol->result = protocoljson_result__error_data_structure_mistake;
			}
			protocol->stage = protocoljson_stage__response_send;
			break;
			
		case protocoljson_stage__command_execute:
			protocol->result = protocoljson_result__error_command_execute;
			protocol->stage = protocoljson_stage__response_send;
			break;

		case protocoljson_stage__response_send:
			/// Response not sended. Think about analyse of this situation
			protocol->stage = protocoljson_stage__init;
			break;
			
		default:
			break;
	} //.switch( protocol->stage )
}
//.protocoljson__stage_timeout()

void protocoljson__timeout_update( ulong systick, protocoljson_t *protocol, void(*stage_timeout_cb)(protocoljson_t *protocol) )
{
	static ulong stage_start_systick;
	static protocoljson_stage_t stage_prev;
	
	ushort stage_timeout = protocoljson_stage__timeouts_ms[ protocol-> stage ];
	if( (stage_timeout == 0) || (protocol->stage != stage_prev) )
	{
		stage_start_systick = systick;
		stage_prev = protocol->stage;
		return;
	}
	stage_prev = protocol->stage;
	
	
	ulong stage_time = (systick > stage_start_systick) ? (systick - stage_start_systick) : (stage_start_systick - systick);
	if( stage_time > stage_timeout )
	{
		stage_timeout_cb( protocol );
	}
}
//.protocoljson__timeout_update()



void  protocoljson_command_get( protocoljson_t *protocol )
{
	protocol->method 		= protocoljson_method__undefined;
	protocol->command_ind 	= 0; // 0 - command_undefinen

	protocoljson_parameter_t 	tmp_parameter;
	ring_buffer_t 			tmp_buffer = {
		.data 	= protocol->buffer.data,
		.size 	= protocol->buffer.size,
		.read 	= protocol->buffer.read,
		.write 	= protocol->buffer.write
	};
	

	while( protocoljson_parameter_get( &tmp_buffer, &tmp_parameter ) == protocoljson__error_none )
	{
		for( uchar i = 1; i < protocoljson_method__count; i++ )
		{
			if(  rbuff_compare_str( protocoljson_method__names[i], &tmp_parameter.name )  )
			{
				protocol->method = (protocoljson_method_t) i;
				for( uchar i = 1; i < protocol->command__count; i++ )
				{
					if(  rbuff_compare_str(  protocol->command__names[i], &tmp_parameter.value )  )
					{
						protocol->command_ind = i;
						break;
					}
				}
				break;
			}
		}

		if( protocol->method != protocoljson_method__undefined )
		{
			break;
		}
	}
}
//.protocoljson__get_command()

//.protocoljson private
//=============================================================================
