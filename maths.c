/* 
 * File:	maths.c
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#include "maths.h"





float round_float( float value, float round )
{
	if( abs_float(value) < round ) return 0.0f;

	int div_value = (int)(value / round);
	
	return div_value * round;
}



double abs_double( double value )
{
	if( value < 0 )
	{
		return value * (-1.0f);
	}
	
	return value;
}

float abs_float( float value )
{
	if( value < 0 )
	{
		return value * (-1.0f);
	}
	
	return value;
}

short abs_short( short value )
{
	if( value < 0 )
	{
		return value * (-1);
	}
	
	return value;
}

long abs_long( long value )
{
	if( value < 0 )
	{
		return value * (-1);
	}
	
	return value;
}

void abs_set_float( float *value, float new_value )
{
	if( *value < 0 )
	{
		*value = new_value * (-1);
	}
	else
	{
		*value = new_value;
	}
}



void limit_uchar( unsigned char *number, unsigned char max, unsigned char min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_short( short *number, short max, short min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_ushort( unsigned short *number, unsigned short max, unsigned short min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_ulong( unsigned long *number, unsigned long max, unsigned long min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_long( long *number, long max, long min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_float( float *number, float max, float min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}

void limit_abs_uchar( unsigned char *number, unsigned char max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_short( short *number, short max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}

void limit_abs_ushort( unsigned short *number, unsigned short max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}

void limit_abs_ulong( unsigned long *number, unsigned long max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}

void limit_abs_long( long *number, long max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}

void limit_abs_float( float *number, float max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}





float deg_to_rad( float angle_degree )
{
	return ( angle_degree * pi ) / 180.0f;
}

float rad_to_deg( float angle_rad )
{
	return ( angle_rad * 180.0f ) / pi;
}


	
float angle_180( float angle_degree )
{
	short  revolutions = (short)( angle_degree / 360.0f );
	float return_angle = angle_degree - (revolutions * 360.0f);	
	
	if( return_angle > 180 ) return_angle = return_angle - 360;
	else if( return_angle < -180 ) return_angle = return_angle + 360;
	
	return return_angle;
}

float angle_360( float angle_degree )
{
	short revolutions = (short)( abs_float(angle_degree) / 360.0f );
	float return_angle = angle_degree - (revolutions * 360.0f);
	
	return return_angle;
}

float angle_360_positive( float angle_degree )
{
	short revolutions = (short)(angle_degree / 360.0f);
	float return_angle = angle_degree - (revolutions * 360.0f);
	
	if( return_angle < 0 ) return_angle += 360.0f;
	
	return return_angle;
}





unsigned char random_get( void )
{
	static unsigned char rand_a;
	static unsigned char rand_b;
	static unsigned char rand_c;
	static unsigned char rand_x;

	rand_x = 0x77; //random value
	rand_a = (rand_a^rand_c^rand_x);
	rand_b = (rand_b + rand_a);
	rand_c = (rand_c + ((rand_b>>1) ^ rand_a));
	
	return rand_c;
}

unsigned char random_seed_get( unsigned char seed )
{
	static unsigned char rand_a;
	static unsigned char rand_b;
	static unsigned char rand_c;

	rand_a = (rand_a^rand_c^seed);
	rand_b = (rand_b + rand_a);
	rand_c = (rand_c + ((rand_b>>1) ^ rand_a));
	
	return rand_c;
}
