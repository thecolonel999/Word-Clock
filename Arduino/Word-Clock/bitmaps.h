/*
 * bitmaps.h
 *
 * Created: 07/10/2015
 * Author: David
 */ 


#ifndef BITMAPS_H_
#define BITMAPS_H_

#include <avr/pgmspace.h>

const uint8_t Hour_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x01, 0x08,
	0x10, 0x81,
	0x08, 0x1f,
	0x81, 0x08,
	0x10, 0x81,
	0x08, 0x00,
	0x00, 0x00,
};


const uint8_t Min_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x03, 0xdc,
	0x22, 0x22,
	0x22, 0x22,
	0x22, 0x22,
	0x22, 0x22,
	0x22, 0x00,
	0x00, 0x00,
};

const uint8_t Circle_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x0f,
	0x01, 0xf8,
	0x3f, 0xc3,
	0xfc, 0x3f,
	0xc3, 0xfc,
	0x1f, 0x80,
	0xf0, 0x00,
	0x00, 0x00, 
};

const uint8_t Set_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00, 
	0x00, 0x00, 
	0x27, 0x02, 
	0x80, 0x78, 
	0x32, 0x64, 
	0xa1, 0x7a, 
	0x14, 0x2e, 
	0x39, 0x00, 
	0x00, 0x00, 
};

const uint8_t C_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00, 
	0x78, 0x08, 
	0x01, 0x00, 
	0x10, 0x01, 
	0x00, 0x10, 
	0x00, 0x80, 
	0x07, 0x80, 
	0x00, 0x00, 
	0x03, 0xfc, 
};

const uint8_t Arrow_bw_bmp[] PROGMEM = {
	12,
	12,
	0x06, 0x00, 
	0x60, 0x06, 
	0x00, 0x60, 
	0x06, 0x0c, 
	0x63, 0xe6, 
	0x77, 0x6e, 
	0x3f, 0xc1, 
	0xf8, 0x0f, 
	0x00, 0x60, 
};


const uint8_t A_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x06,
	0x00, 0x90,
	0x10, 0x81,
	0x08, 0x10,
	0x81, 0xF8,
	0x10, 0x81,
	0x08, 0x10,
	0x80, 0x00,
};

const uint8_t bars_1_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x40,
	0x00, 0x00,
};

const uint8_t bars_2_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x10, 0x01,
	0x00, 0x50,
	0x00, 0x00,
};

const uint8_t bars_3_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x04,
	0x00, 0x40,
	0x14, 0x01,
	0x40, 0x54,
	0x00, 0x00,
};

const uint8_t bars_4_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x01, 0x00,
	0x10, 0x05,
	0x00, 0x50,
	0x15, 0x01,
	0x50, 0x55,
	0x00, 0x00,
};

const uint8_t bars_5_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x00, 0x00,
	0x40, 0x04,
	0x01, 0x40,
	0x14, 0x05,
	0x40, 0x54,
	0x15, 0x41,
	0x54, 0x55,
	0x40, 0x00,
};

const uint8_t Brightness_bw_bmp[] PROGMEM = {
	12,
	12,
	0x00, 0x00,
	0x40, 0x40,
	0x20, 0xF0,
	0x10, 0x82,
	0x04, 0xA0,
	0x52, 0x04,
	0x10, 0x80,
	0xF0, 0x40,
	0x20, 0x20,
};


#endif /* BITMAPS_H_ */