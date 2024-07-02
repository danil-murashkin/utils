/* 
 * File:	maths.h
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#ifndef __MATHS_H
#define	__MATHS_H

#ifdef __cplusplus
extern "C" {
#endif



	static const float pi = 3.14159265359;
	
	
	
	float round_float( float value, float round );

	double abs_double( double value );
	float abs_float( float value );
	short abs_short( short value );
	long abs_long( long value );
	void abs_set_float( float *value, float new_value );

	void limit_uchar( unsigned char *number, unsigned char max, unsigned char min );
	void limit_short( short *number, short max, short min );
	void limit_ushort( unsigned short *number, unsigned short max, unsigned short min );
	void limit_ulong( unsigned long *number, unsigned long max, unsigned long min );
	void limit_long( long *number, long max, long min );
	void limit_float( float *number, float max, float min );
	void limit_abs_uchar( unsigned char *number, unsigned char max_abs );
	void limit_abs_short( short *number, short max_abs );
	void limit_abs_ushort( unsigned short *number, unsigned short max_abs );
	void limit_abs_ulong( unsigned long *number, unsigned long max_abs );
	void limit_abs_long( long *number, long max_abs );
	void limit_abs_float( float *number, float max_abs );


	float deg_to_rad( float angle_degree );
	float rad_to_deg( float angle_rad );
	
	float angle_180( float angle_degree );
	float angle_360( float angle_degree );
	float angle_360_positive( float angle_degree );
	
	
	unsigned char random_get( void );
	unsigned char random_seed_get( unsigned char seed );



#ifdef __cplusplus
}
#endif

#endif //.__MATHS_H
