/* 
 * File:	functions.h
 * Author:  Danil Murashkin
 * Created: 2018.06.12 23:55
 */

#ifndef __FUNCTIONS_H
#define	__FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif



	#define FUNC__DENY 				0
	#define FUNC__CONFIRM 			1
	#define FUNC__TRY_AGAIN 		2		
	#define FUNC__FINISH 			3

	#define ERROR__NONE				0
	#define ERROR__HAVE				-1

	#define RESULT__CONFIRM 		0
	#define RESULT__DENY 			-1



#ifdef __cplusplus
}
#endif

#endif //.__FUNCTIONS_H
