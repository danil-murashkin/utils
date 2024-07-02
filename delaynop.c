/* 
 * File:    delaynop.h
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#include "delaynop.h"



static unsigned char	nop_delay_1us 	= 8;
static unsigned char	nop_delay_10us 	= 80;
static unsigned short	nop_delay_100us = 800;
static unsigned short	nop_delay_1ms 	= 8000;
static unsigned long	nop_delay_10ms 	= 80000;
static unsigned long	nop_delay_100ms = 800000;
static unsigned long	nop_delay_1s 	= 8000000;





void delaynop_config( unsigned char ticks_1us, unsigned char ticks_10us, unsigned short ticks_100us, 
        unsigned short ticks_1ms, unsigned long ticks_10ms, unsigned long ticks_100ms, 
        unsigned long ticks_1s )
{
	nop_delay_1us 	= ticks_1us;
	nop_delay_10us 	= ticks_10us;
	nop_delay_100us = ticks_100us;

	nop_delay_1ms 	= ticks_1ms;
	nop_delay_10ms 	= ticks_10ms;
	nop_delay_100ms = ticks_100ms;

	nop_delay_1s 	= ticks_1s;
}
//.delaynop_config()



void delaynop_us( unsigned short time )
{	
	unsigned char time_try_1 = time % 10;
	for( unsigned char i = 0; i < nop_delay_1us * time_try_1; i++ ) asm("NOP");
	if( time < 10 ) return;
		
	unsigned char time_try_10 = time %100 / 10;
	for( unsigned short i = 0; i < nop_delay_10us * time_try_10; i++ ) asm("NOP");
	if( time < 100 ) return;
	
	unsigned char time_try_100 = time / 100;
	for( unsigned long i = 0; i < nop_delay_100us * time_try_100; i++ ) asm("NOP");
}
//.delaynop_us()

void delaynop_ms( unsigned short time )
{
	unsigned char time_try_1 = time % 10;
	for( unsigned long i = 0; i < nop_delay_1ms * time_try_1; i++ ) asm("NOP");
	if( time < 10 ) return;
		
	unsigned char time_try_10 = time %100 / 10;
	for( unsigned long i = 0; i < nop_delay_10ms * time_try_10; i++ ) asm("NOP");
	if( time < 100 ) return;
	
	unsigned char time_try_100 = time / 100;
	for( unsigned long i = 0; i < nop_delay_100ms * time_try_100; i++ ) asm("NOP");
}
//.delaynop_ms()

void delaynop_s( unsigned char time )
{
	for( unsigned char try_1s = 0; try_1s < time; try_1s++ )
		for( unsigned long i = 0; i < nop_delay_1s; i++ ) asm("NOP");
}
//.delaynop_s()
