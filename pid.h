/* 
 * File:	pid.h
 * Author:  Danil Murashkin
 * Created: 2020.08.04 09:13
 */

#ifndef __PID_H
#define	__PID_H

#ifdef __cplusplus
extern "C" {
#endif



	typedef struct // pid_settings_t
	{
		float accuracity;
		
		float Kp;
		float Ki;
		float Kd;
		
		float error_limit;
		float integral_limit;
		float output_limit;
		float output_begin;
	} pid_settings_t;
	
	
	typedef struct // pid_data_t
	{
		float accuracity;
		
		float Kp;
		float Ki;
		float Kd;
		
		float error_limit;
		float integral_limit;
		float output_limit;
		float output_begin;
		
		float error_last;
		float integral_last;		
	} pid_data_t;



	void pid_setup( pid_data_t *pid, pid_settings_t settings );
	void pid_clear( pid_data_t *pid );

	float pid_update( pid_data_t *pid, float error );



#ifdef __cplusplus
}
#endif

#endif //.__PID_H
