/* 
 * File:	strings.c
 * Author:  Danil Murashkin
 * Created: 2020.06.25 04:03
 */

#include "strings.h"

#include "functions.h"
#include "types.h"





//=============================================================================
// String operations

unsigned char string_found_substring( unsigned short *substring_index, unsigned char* substring, unsigned short substring_len, unsigned char* string_, unsigned short string_len ) // TRUE or FALSE
{
	unsigned short i = 0;
	while( (string_[i]) && (i < string_len) && ((substring_len + i) <= string_len) )
	{
		if( string_[i] == substring[0] )
		{
			unsigned short j = 0;
			while( (string_[i+j]) && (substring[j]) && (string_[i+j] == substring[j]) && (j < substring_len) )
			{
				j++;
			}
			
			if( j == substring_len )
			{
				*substring_index = i;
				return TRUE;
			}
		}
		
		i++;
	}
	
	return FALSE;
}

unsigned char string_found_char( unsigned short *char_index, unsigned char char_, unsigned char* string_, unsigned short string_len ) // TRUE or FALSE
{
	unsigned short i = 0;
	while( (string_[i]) && (i < string_len) )
	{
		if( string_[i] == char_ )
		{
			*char_index = i;
			return TRUE;
		}
		
		i++;
	}
	
	return FALSE;
}

void string_cut( unsigned short index, unsigned short cut_len, unsigned char* string_,  unsigned short *string_len )
{
	if( (cut_len == 0) || (*string_len == 0) || ((cut_len + index) > *string_len) ) return;
	
	const unsigned char null_symbol = 0x00;
	for( unsigned short i = index; i < index + *string_len - cut_len; i++ ) string_[ i ] = string_[ i+cut_len ];
	for( unsigned short i = *string_len - cut_len; i < *string_len; i++ ) string_[ i ] = null_symbol;
	
	*string_len -= cut_len;
}
//.string_cut()

void string_shift( unsigned short index, short shift, unsigned char* string_, unsigned short *string_len, unsigned short string_size )
{
	if( (shift == 0) || (*string_len == 0) || (index >= *string_len) ) return;
	
	const unsigned char null_symbol = 0x00;	
	if( shift > 0 )
	{
		if( ((*string_len - index) + shift) > string_size ) return;
		
		for( unsigned short i = *string_len; i > index; i-- ) string_[ i-1 + shift  ] = string_[ i-1 ];
		for( unsigned short i = index; i < index + shift; i++ ) string_[ i ] = null_symbol;
		
		*string_len = *string_len + shift;
	}
	else
	{
		if( ((-1*shift) > string_size) || ((-1*shift) > *string_len) || ((-1*shift) > index) || (index == 0) ) return;
		
		for( unsigned short i = 0; i < index+1 + shift; i++ ) string_[ i ] = string_[ i-shift ];
		for( unsigned short i = index+1 + shift; i < index+1; i++ ) string_[ i ] = null_symbol;
		
		if( index == (*string_len-1) ) *string_len = *string_len + shift;
	}	
}
//.string_shift()

unsigned char strings_compare( unsigned char* first, unsigned char* second, unsigned short length ) // TRUE or FALSE
{
	unsigned short i = 0;
	while( first[i] && second[i] && (i < length) )
	{
		if( first[i] != second[i] )
		{
			return FALSE;
		}

		i++;
	}

	if( i == length )
	{
		return TRUE;
	}
  	else
  	{
  		return FALSE;
  	}
}
//.strings_compare()

void string_insert_string( unsigned char* insert_string, unsigned short insert_string_len, 
						   unsigned short insert_index, unsigned char* string_, unsigned short *string_len, unsigned short string_size )
{
	if( (insert_index + insert_string_len) > string_size ) return;
	
	for( unsigned short i = 0; i < insert_string_len; i++ )
	{
		string_[ insert_index +  i ] = insert_string[ i ];
	}
	
	unsigned short temp_len = insert_index + 1 + insert_string_len;
	if( *string_len < temp_len ) *string_len = temp_len;
} 
//.string_insert_string()

void string_insert_char( unsigned char insert_char, unsigned short insert_index, 
						 unsigned char* string_, unsigned short *string_len, unsigned short string_size )
{
	if( insert_index > (string_size-1) ) return;
	
	string_[ insert_index ] = insert_char;

	unsigned short temp_len = insert_index + 1;
	if( *string_len < temp_len ) *string_len = temp_len;
} 
//.string_insert_char()

//.String operations
//=============================================================================



//=============================================================================
// Value convert to string

void float_to_string( float number_in, unsigned char* string_out, unsigned short *string_out_len )
{
    *string_out_len = 0;
}

void long_to_string( long number_in, unsigned char* string_out, unsigned short *string_out_len )
{
	if( number_in == 0 ) 
	{
		string_out[0] = 48; //ASCII '0'
		*string_out_len = 1;
		
		return;
	} 
	
	
    *string_out_len = 0;
	char less_zero_k = 1;
	if( number_in < 0 ) 
	{
		string_out[ *string_out_len ] = '-';
		string_out_len += 1;
		less_zero_k = -1;
	}
	

	unsigned long digit = 1000000000;
	
	while( digit >= 1 )
	{
		if( number_in >= digit )
		{
			string_out[ *string_out_len ] = 48 + (( number_in * less_zero_k ) / digit) % 10;
			*string_out_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}
} 
//.long_to_string

void ulong_to_string( unsigned long number_in, unsigned char* string_out, unsigned short *string_out_len )
{
	if( number_in == 0 ) 
	{
		string_out[0] = 48; //ASCII '0'
		*string_out_len = 1;
		
		return;
	} 
	

    *string_out_len = 0;
	unsigned long digit = 1000000000;
	
	while( digit >= 1 )
	{
		if( number_in >= digit )
		{
			string_out[ *string_out_len ] = 48 + (number_in / digit) % 10;
			*string_out_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} 
//.ulong_to_string

void short_to_string( short number_in, unsigned char* string_out, unsigned short *string_out_len )
{
	if( number_in == 0 ) 
	{
		string_out[0] = 48; //ASCII '0'
		*string_out_len = 1;
		
		return;
	} 
	
    
	*string_out_len = 0;
	char less_zero_k = 1;
	if( number_in < 0 ) 
	{
		string_out[ *string_out_len ] = '-';
		string_out_len += 1;
		less_zero_k = -1;
	}
	

	unsigned short digit = 10000;
	
	while( digit >= 1 )
	{
		if( number_in >= digit )
		{
			string_out[ *string_out_len ] = 48 + (( number_in * less_zero_k ) / digit) % 10;
			*string_out_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}
} 
//.short_to_string

void ushort_to_string( unsigned short number_in, unsigned char* string_out, unsigned short *string_out_len )
{
	if( number_in == 0 ) 
	{
		string_out[0] = 48; //ASCII '0'
		*string_out_len = 1;
		
		return;
	} 
	
    
    *string_out_len = 0;
	unsigned short digit = 10000;
	
	while( digit >= 1 )
	{
		if( number_in >= digit )
		{
			string_out[ *string_out_len ] = 48 + (number_in / digit) % 10;
			*string_out_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} 
//.ushort_to_string

void uchar_to_string( unsigned char number_in, unsigned char*string_out, unsigned short *string_out_len )
{
	if( number_in == 0 ) 
	{
		string_out[0] = 48; //ASCII '0'
		*string_out_len = 1;
		
		return;
	} 
	
    *string_out_len = 0;
	unsigned char digit = 100;
	
	while( digit >= 1 )
	{
		if( number_in >= digit )
		{
			string_out[ *string_out_len ] = 48 + (number_in / digit) % 10;
			*string_out_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} 
//.uchar_to_string

//.Value convert to string
//=============================================================================



//=============================================================================
// String convert to value

unsigned char string_to_float( unsigned char* string_in, unsigned short string_in_len, float *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	float number = 0;
	unsigned char math_sign_neg_f = FALSE;

	unsigned char integral_part_f = FALSE;	
	unsigned long integral_value = 0;
	unsigned long integral_digit = 1;
	const unsigned long integral_digit_max = 100000000;
	float fractional_value = 0;
	unsigned long fractional_digit = 1;
	const unsigned long fractional_digit_max = 100000000;	
	
	unsigned char numeral = 0;	
	unsigned char temp_uchar;
	unsigned short i = string_in_len;	
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			
			numeral = temp_uchar - 48;
			
			if( integral_part_f == FALSE )
			{		
				if( (fractional_digit == fractional_digit_max) && (numeral > 2) && (fractional_value > 147483647) )
				{
					return FUNC__DENY;
				}
				
				fractional_value += numeral * fractional_digit;
				
				if( fractional_digit == fractional_digit_max )
				{
					break;
				}
				else 
				{
					fractional_digit *= 10;
				}
			}
			else
			{			
				if( (integral_digit == integral_digit_max) && (numeral > 2) && (integral_value > 147483647) )
				{
					return FUNC__DENY;
				}
				
				integral_value += numeral * integral_digit;
				
				if( integral_digit == integral_digit_max )
				{
					break;
				}
				else 
				{
					integral_digit *= 10;
				}
			}
			
		}
		else
		{
			
			if( (temp_uchar == 46) && (integral_part_f == FALSE) ) integral_part_f = TRUE;
			
			if( (fractional_digit != 1) && (temp_uchar == 45) ) 
			{
				math_sign_neg_f = TRUE;
				break;
			}
			
		}
	}//while( i-- ) 
	

	if( integral_part_f == FALSE )
	{
		number = fractional_value;
	}
	else
	{
		number = integral_value;		
		number += (fractional_value / fractional_digit);
	}
	if( math_sign_neg_f ) number *= -1;
	

	*number_out =  number;
	return FUNC__CONFIRM;
}
//.string_to_float()

unsigned char string_to_long( unsigned char* string_in, unsigned short string_in_len, long *number_out) // return FUNC__DENY or FUNC__CONFIRM
{
	//long max value from -2147483647 to +2147483647
	long number = 0;
	unsigned char math_sign_neg_f = FALSE;

	unsigned long digit = 1;
	const unsigned long digit_max = 1000000000;
	
	unsigned char numeral = 0;
	unsigned char temp_uchar;
	unsigned short i = string_in_len;
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;			
			if( (digit == digit_max) && (numeral > 2) && (number > 147483647) )
			{
				return FUNC__DENY;
			}
			
			number += numeral * digit;
			
			if( digit == digit_max )
			{
				break;
			}
			else
			{
				digit *= 10;
			}
		}
		else
		{
			if( (digit != 1) && (temp_uchar == 45) ) 
			{
				math_sign_neg_f = TRUE;
				break;
			}
		}
	}
	
	if( math_sign_neg_f ) number *= -1;
	

	*number_out = number;
	return FUNC__CONFIRM;
}

unsigned char string_to_ulong( unsigned char* string_in, unsigned short string_in_len, unsigned long *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	//ulong max value 4294967295
	unsigned long number = 0;

	unsigned long digit = 1;
	const unsigned long digit_max = 1000000000;
	
	unsigned char numeral = 0;
	unsigned char temp_uchar;
	unsigned short i = string_in_len;
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;
			if( (digit == digit_max) && (numeral > 4) && (number > 294967295) )
			{
				return FUNC__DENY;
			}
			
			number += numeral * digit;
			
			if( digit == digit_max ) 
			{
				break;
			}
			else
			{
				digit *= 10;
			}
		}
		else
		{
			if( digit != 1 )
			{
				return FUNC__DENY;
			}
		}
	}
	

	*number_out = number;
	return FUNC__CONFIRM;
}

unsigned char string_to_short( unsigned char* string_in, unsigned short string_in_len, short *number_out) // return FUNC__DENY or FUNC__CONFIRM
{
	//short max value from -32767 to +32767
	short number = 0;
	unsigned char math_sign_neg_f = FALSE;

	unsigned short digit = 1;
	const unsigned short digit_max = 10000;
	
	unsigned char numeral = 0;
	unsigned char temp_uchar;
	unsigned short i = string_in_len;
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;			
			if( (digit == digit_max) && (numeral > 3) && (number > 2767) )
			{
				return FUNC__DENY;
			}
			
			number += numeral * digit;
			
			if( digit == digit_max )
			{
				break;
			}
			else
			{
				digit *= 10;
			}
		}
		else
		{
			if( (digit != 1) && (temp_uchar == 45) ) 
			{
				math_sign_neg_f = TRUE;
				break;
			}
		}
	}
	
	if( math_sign_neg_f ) number *= -1;
	

	*number_out = number;
	return FUNC__CONFIRM;
}
//.string_to_short()

unsigned char string_to_ushort( unsigned char* string_in, unsigned short string_in_len, unsigned short *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	//ushort max value 65535
	unsigned short number = 0;

	unsigned short digit = 1;
	const unsigned short digit_max = 10000;
	
	unsigned char numeral = 0;
	unsigned char temp_uchar;
	unsigned short i = string_in_len;
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;
			if( (digit == digit_max) && (numeral > 6) && (number > 5535) )
			{
				return FUNC__DENY;
			}
			
			number += numeral * digit;
			
			if( digit == digit_max ) 
			{
				break;
			}
			else
			{
				digit *= 10;
			}
		}
		else
		{
			if( digit != 1 )
			{
				return FUNC__DENY;
			}
		}
	}
	

	*number_out = number;
	return FUNC__CONFIRM;
}
//.string_to_ushort()

unsigned char string_to_uchar( unsigned char* string_in, unsigned short string_in_len, unsigned char *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	//uchar max value 255
	unsigned char number = 0;

	unsigned char digit = 1;
	const unsigned char digit_max = 100;

	unsigned char numeral = 0;
	unsigned char temp_uchar;
	unsigned short i = string_in_len;
	
	while( i-- ) 
	{
		temp_uchar = string_in[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;			
			if( (digit == digit_max) && (numeral > 2) && (number > 55) ) 
			{
				return FUNC__DENY;
			}
			
			number += numeral * digit;
			
			if( digit == digit_max ) 
			{
				break;
			}
			else
			{
				digit *= 10;
			}
		}
		else
		{
			if( digit != 1 ) 
			{
				return FUNC__DENY;
			}
		}
	}
	

	*number_out = number;
	return FUNC__CONFIRM;
}
//.string_to_uchar()

//.String convert to value
//=============================================================================



//=============================================================================
// Hex string convert to value

unsigned char string_hex_to_uchar( unsigned char* string_in, unsigned short string_in_len, unsigned char *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	unsigned char number = 0x00;

	unsigned char digit_hex = 2; // 2 - 7b..4b, 1 - 3b..0b
	unsigned char i = 0;
	while( (string_in[i]) && (i<string_in_len) && (digit_hex>0) )
	{

		// 30 - 0, 39 - 9, 41 - A, 46 - F
		char symbol = string_in[i];
		if( (symbol >= '0') && (symbol <= '9') )
		{
		    symbol = symbol - '0';
		}
		else if( (symbol >= 'A') && (symbol <= 'F') )
		{
		    symbol = symbol - 'A' + 10;
		}
		else
		{
		    return FUNC__DENY;
		}
		
		number += (symbol) << (4*(digit_hex-1));

		
	    digit_hex--;	
		i++;
	}
	

	*number_out = number;
	return FUNC__CONFIRM;
}
//.string_hex_to_uchar()

unsigned char string_hex_to_ushort( unsigned char* string_in, unsigned short string_in_len, unsigned short *number_out ) // return FUNC__DENY or FUNC__CONFIRM
{
	unsigned short number = 0x00;

	unsigned char digit_hex = 4; // 2 - 7b..4b, 1 - 3b..0b
	unsigned char i = 0;
	while( (string_in[i]) && (i<string_in_len) && (digit_hex>0) )
	{

		// 30 - 0, 39 - 9, 41 - A, 46 - F
		char symbol = string_in[i];
		if( (symbol >= '0') && (symbol <= '9') )
		{
		    symbol = symbol - '0';
		}
		else if( (symbol >= 'A') && (symbol <= 'F') )
		{
		    symbol = symbol - 'A' + 10;
		}
		else
		{
		    return FUNC__DENY;
		}
		
		number += (symbol) << (4*(digit_hex-1));

		
	    digit_hex--;	
		i++;
	}
	

	*number_out = number;
	return FUNC__CONFIRM;
}
//.string_hex_to_ushort()

//.Hex string convert to value
//=============================================================================