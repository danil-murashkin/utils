/* 
 * File:    delaynop.h
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#ifndef __DELAYNOP_H
#define	__DELAYNOP_H

#ifdef __cplusplus
extern "C" {
#endif



    void delaynop_config( unsigned char ticks_1us, unsigned char ticks_10us, unsigned short ticks_100us, 
        unsigned short ticks_1ms, unsigned long ticks_10ms, unsigned long ticks_100ms, 
        unsigned long ticks_1s );
    // example: delaynop_config( 8, 80, 800, 8000, 80000, 800000, 8000000 );
    
    void delaynop_us( unsigned short time );
    void delaynop_ms( unsigned short time );
    void delaynop_s ( unsigned char time );



#ifdef __cplusplus
}
#endif

#endif //.__DELAYNOP_H
