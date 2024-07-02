/* 
 * File:	jsons.h
 * Author:  Danil Murashkin
 * Created: 2024.05.17 18:44
 */

#ifndef __JSONS_H
#define	__JSONS_H

#ifdef __cplusplus
extern "C" {
#endif



	#include "types.h"
	
	
	
	typedef enum { // jsons_value_type_t 
		jsons_value_type__undefined = 0,
		jsons_value_type__number,
		jsons_value_type__string,
		jsons_value_type__array_numbers,
		jsons_value_type__array_strings,
		jsons_value_type__json_structure
	} jsons_value_type_t;
	
	static const unsigned char jsons_skip_symbols[] = { ' ', '\n', '\r', '\0', '\t'};
	static const unsigned char jsons_skip_symbols_len = sizeof(jsons_skip_symbols);


	int  jsons_brackets_get_rbuff( ring_buff_uchar_t rbuff,  unsigned short *open_index, unsigned short *close_index );
	int  jsons_brackets_get( unsigned char* data, unsigned short data_len,  unsigned short *open_index, unsigned short *close_index );

	int  jsons_parameter_get( unsigned char* data, unsigned short data_len,
		unsigned char* parameter_name, unsigned short parameter_name_len, 
		jsons_value_type_t *value_type, unsigned char **value, unsigned short *value_len );




#ifdef __cplusplus
}
#endif

#endif //.__JSONS_H
