/* 
 * File:	pid.c
 * Author:  Danil Murashkin
 * Created: 2020.08.04 09:13
 */

//http://we.easyelectronics.ru/Theory/pid-regulyatory--dlya-chaynikov-praktikov.html

#include "pid.h"
#include "maths.h"





float pid_update( pid_data_t *pid, float error )
{
	//u(t) = P (t) + I (t) + D (t);
	//P (t) = Kp * e (t);
	//I (t) = I (t — 1) + Ki * e (t);
	//D (t) = Kd * {e (t) — e (t — 1)};

	if( abs_float( error ) < pid->accuracity ) 
	{
		error = 0.0f;
	}
	
	if( (pid->error_limit > 0) && (abs_float(error) > pid->error_limit) ) error = (error > 0) ? pid->error_limit : -1*pid->error_limit;


	float output = 0.0f;

	float proportional = pid->Kp * error;
	float integral = pid->integral_last + pid->Ki * error;
	float derivative = pid->Kd * ( error - pid->error_last );

	if( (pid->integral_limit > 0) && (abs_float(integral) > pid->integral_limit) ) integral = (integral > 0) ? pid->integral_limit : -1*pid->integral_limit;
	
	output = proportional + integral + derivative;


	pid->error_last = error;
	pid->integral_last = integral;
	
	if( output != 0 ) output += (output > 0) ? pid->output_begin : -1*pid->output_begin;
	if( (pid->output_limit > 0) && (abs_float(output) > pid->output_limit) ) output = (output > 0) ? pid->output_limit : -1*pid->output_limit;

	
	return output;
}



void pid_setup( pid_data_t *pid, pid_settings_t settings )
{
	pid->Kp = settings.Kp;
	pid->Ki = settings.Ki;
	pid->Kd = settings.Kd;
	
	pid->accuracity = settings.accuracity;
	
	pid->error_limit = settings.error_limit;
	pid->integral_limit = settings.integral_limit;
	pid->output_limit = settings.output_limit;
	pid->output_begin = settings.output_begin;
	
	pid->error_last = 0;
	pid->integral_last = 0;
}


void pid_clear( pid_data_t *pid )
{
	pid->error_last = 0;
	pid->integral_last = 0;
}
