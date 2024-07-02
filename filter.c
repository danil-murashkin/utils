/* 
 * File:	filter.h
 * Author:  Danil Murashkin
 * Created: 2022.04.01
 */

#include "filter.h"





float value_filtered_exponential_fit_update( float value_new, float *value_filtered_prev, float filter_k ) //value_filtered_new
{
	//Yn = (Yn-1 * K) + Xn * (1-K) exponential fit
	float value_filtered_new = (*value_filtered_prev * filter_k) + value_new * (1-filter_k);

	*value_filtered_prev = value_filtered_new;
	return value_filtered_new;
}

void velocity_by_position_filtered_exponential_fit_update( float time, float angle, float *angle_prev, float *velocity_by_position, float *velocity_by_position_prev, float filter_k )
{
	float velocity_by_position_new;
	
	// -180 -> 180 issue
	if( (angle - *angle_prev) < -240.0f ) 
		velocity_by_position_new = (angle - (*angle_prev-360.0f)) / time;
	else if( (angle - *angle_prev) > 240.0f ) 
		velocity_by_position_new = ((angle-360.0f) - *angle_prev) / time;	
	else
		velocity_by_position_new = (angle - *angle_prev) / time;
	
	
	//Yn = (Yn-1 * K) + Xn * (1-K) exponential fit
	velocity_by_position_new = (*velocity_by_position_prev * filter_k) + (velocity_by_position_new*(1-filter_k));
	
	
	*angle_prev = angle;
	*velocity_by_position = velocity_by_position_new;
	*velocity_by_position_prev = velocity_by_position_new;
}
