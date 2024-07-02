/* 
 * File:	filter.h
 * Author:  Danil Murashkin
 * Created: 2022.04.01
 */

#ifndef __FILTER_H
#define	__FILTER_H

#ifdef __cplusplus
extern "C" {
#endif



	float value_filtered_exponential_fit_update( float value_new, float *value_prev, float filter_k ); //value_filtered
	void velocity_by_position_filtered_exponential_fit_update( float time, float angle, float *angle_prev, float *velocity_by_position, float *velocity_by_position_prev, float filter_k );



#ifdef __cplusplus
}
#endif

#endif //.__FILTER_H
