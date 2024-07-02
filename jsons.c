/* 
 * File:	jsons.c
 * Author:  Danil Murashkin
 * Created: 2024.05.17 18:44
 */

#include "jsons.h"

#include "types.h"
#include "functions.h"
#include "strings.h"



// jsons private block
int  jsons_skip_empty_symbols( unsigned char* data, unsigned short data_len, unsigned short data_offset,  unsigned short *skip_index );





//=============================================================================
// jsons public block

int  jsons_parameter_get( unsigned char* data, unsigned short data_len,
		unsigned char* parameter_name, unsigned short parameter_name_len, 
		jsons_value_type_t *value_type, unsigned char **value, unsigned short *value_len )
{
	*value_type = jsons_value_type__undefined;
	
	unsigned short parameter_index = 0;
	if(  !string_found_substring( &parameter_index, parameter_name, parameter_name_len, data, data_len )  )
	{
		return FALSE;
	}
	
	unsigned short parameter_value_index = 0;
	jsons_skip_empty_symbols( data, data_len, (parameter_index + parameter_name_len),  &parameter_value_index );
	if( data[ parameter_value_index ] !=':' )
	{
		return FALSE;
	}
	
	if( (data_len - parameter_value_index) < 2 )
	{
		return FALSE;
	}
	parameter_value_index += 1;	// goto next symboll after ':'
	
	
	unsigned short value_begin_index = 0;
	unsigned short value_end_index = 0;
	jsons_skip_empty_symbols( data, data_len, parameter_value_index,  &value_begin_index );
	
	switch( data[ value_begin_index ] )
	{
		case '}':
		case ',':
		{
			return FALSE;
		}
		break;
		
		
		// jsons_value_type__json_structure
		case '{':
		{
			value_begin_index += 1;	// goto next symboll after '{'
			
			int brackets_count = 1;
			
			for( unsigned short i = value_begin_index; i < data_len; i++ )
			{
				if(      data[i] == '{' ) brackets_count++;
				else if( data[i] == '}' ) brackets_count--;
				
				if( brackets_count == 0 ) 
				{
					value_end_index = i - 1;
					
					*value_type = jsons_value_type__json_structure;
					*value = data + value_begin_index;
					*value_len = value_end_index - value_begin_index + 1;
					
					return TRUE;
				}
			}
			
			if( brackets_count != 0 )
			{
				return FALSE;
			}
		}
		break;
		//.jsons_value_type__json_structure
			
		
		// jsons_value_type__array_strings or jsons_value_type__array_numbers
		case '[':
		{
			value_begin_index += 1;
			/// jsons_value_type__array
			
			// jsons_value_type__array_strings
			
			// jsons_value_type__array_numbers
		}
		break;
		//.jsons_value_type__array_strings or jsons_value_type__array_numbers
		
		
		// jsons_value_type__string
		case '"':
		{
			value_begin_index += 1;
			if(  !string_found_char( &value_end_index, '"', (data + value_begin_index), (data_len - value_begin_index) )  )
			{
				return FALSE;
			}
			else
			{
				value_end_index += value_begin_index - 1;
				
				*value_type = jsons_value_type__string;
				*value = data + value_begin_index;
				*value_len = value_end_index - value_begin_index + 1;
				
				return TRUE;
			}
		}
		//.jsons_value_type__string
	
		
		//jsons_value_type__number
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			unsigned short i;
			for( i = value_begin_index; i < data_len; i++ )
			{
				if( (data[ i ] < '0') || (data[ i ] > '9') ) break;
			}
			
			if( i > value_begin_index )
			{
				value_end_index = i - 1;
						
				*value_type = jsons_value_type__number;
				*value = data + value_begin_index;
				*value_len = value_end_index - value_begin_index + 1;
				
				return TRUE;
			}
		}
		break;
		//.jsons_value_type__number
		
		
		default:
		{
			return FALSE;
		}
		break;
	}
	

	return FALSE;
}
//.jsons_parameter_get()


int  jsons_brackets_get_rbuff( ring_buff_uchar_t rbuff,  unsigned short *open_index, unsigned short *close_index )
{
	///! rbuff jsons_brackets_get_rbuff
	unsigned short open_index_tmp = 0;
	int brackets_count = -1; // error value, means need clear array

	unsigned short i = 0;
	while( i < rbuff.len && rbuff.data[i] )
	{
		if( rbuff.data[i] == '{' ) 
		{
			if( brackets_count == -1 )
			{
				open_index_tmp = i;
				brackets_count = 0;
			}
			brackets_count++;
		}

		else if( rbuff.data[i] == '}' ) 
		{
			if( brackets_count > 0 )
			{
				brackets_count--;
				if( brackets_count == 0 )
				{
					*open_index  = open_index_tmp;
					*close_index = i;
					
					return brackets_count;
				}
			}
		}
		
		i++;
	}
	
	return brackets_count;
}
//.jsons_brackets_get_rbuff()


int  jsons_brackets_get( unsigned char* data, unsigned short data_len,  unsigned short *open_index, unsigned short *close_index )
{
	unsigned short open_index_tmp = 0;
	int brackets_count = -1; // error value, means need clear array

	unsigned short i = 0;
	while( i < data_len && data[i] )
	{
		if( data[i] == '{' ) 
		{
			if( brackets_count == -1 )
			{
				open_index_tmp = i;
				brackets_count = 0;
			}
			brackets_count++;
		}

		else if( data[i] == '}' ) 
		{
			if( brackets_count > 0 )
			{
				brackets_count--;
				if( brackets_count == 0 )
				{
					*open_index  = open_index_tmp;
					*close_index = i;
					
					return brackets_count;
				}
			}
		}
		
		i++;
	}
	
	return brackets_count;
}
//.jsons_brackets_get()

//.jsons public block
//=============================================================================




//=============================================================================
// jsons private block

int  jsons_skip_empty_symbols( unsigned char* data, unsigned short data_len, unsigned short data_offset,  unsigned short *skip_index )
{
	for( unsigned short i = data_offset; i < data_len; i++ )
	{
		unsigned char j;
		for( j = 0; j < jsons_skip_symbols_len; j++ )
		{
			if( data[i] == jsons_skip_symbols[j] )
			{
				break;
			}
		}
		if( j == jsons_skip_symbols_len )
		{
			*skip_index = i; 
			return TRUE;
		}
	}
	
	return FALSE;
}
//.jsons_skip_empty_symbols()

//.jsons private block
//=============================================================================
