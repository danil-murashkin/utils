/* 
 * File:	crc.h
 * Author:  Danil Murashkin
 * Created: 2017.05.12 12:10
 */

#ifndef __CRC_H
#define	__CRC_H

#ifdef __cplusplus
extern "C" {
#endif



	//supported crc calculations:
	//CRC-8/CDMA2000
	//
	//CRC-16/MODBUS
	//CRC-16/XMODEM
	//CRC-16/CRCA
	
	//calculations: https://crccalc.com/
	//tables: https://cs.fit.edu/code/svn/cse2410f13team7/wireshark/wsutil/crc16.c
	
	
	//CRC-8/CDMA2000 (poly: 0x9B, seed: 0xFF, unreflected in and out)
	extern const unsigned char crc8_cdma_init_seed;
	unsigned char crc8_cdma( unsigned char *data_ptr, unsigned char len) ;
	unsigned char crc8_cdma_array( unsigned char *data_ptr, unsigned char len, unsigned char crc_in);
	unsigned char crc8_cdma_byte( unsigned char byte, unsigned char crc_in) ;
	
	
	//CRC-16/MODBUS (poly: 0x8005, seed: 0xFFFF, reflected in and out)
	extern const unsigned short crc16_modbus_init_seed;
	unsigned short crc16_modbus( unsigned char* data, unsigned char len );
	unsigned short crc16_modbus_array( unsigned char* data, unsigned char len, unsigned short crc_in );
	unsigned short crc16_modbus_byte( unsigned char byte, unsigned short crc_in );
	
	//CRC-16/XMODEM (poly: 0x1021, seed: 0x0000, unreflected in and out)
	extern const unsigned short crc16_xmodem_init_seed;
	unsigned short crc16_xmodem( unsigned char* data, unsigned char len );
	unsigned short crc16_xmodem_array( unsigned char* data, unsigned char len, unsigned short crc_in );
	unsigned short crc16_xmodem_byte( unsigned char byte, unsigned short crc_in );
	
	//CRC-16/CRCA (poly: 0x1021, seed: 0x6363, reflected in and out)
	extern const unsigned short crc16_crca_init_seed;
	unsigned short crc16_crca( unsigned char* data, unsigned char len );
	unsigned short crc16_crca_array( unsigned char* data, unsigned char len, unsigned short crc_in );
	unsigned short crc16_crca_byte( unsigned char byte, unsigned short crc_in );



#ifdef __cplusplus
}
#endif

#endif //.__CRC_H
