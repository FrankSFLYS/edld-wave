/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : key.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : To scan key matrix and return specified keycode
http://frank.xin
**********************************************************************/
#include "key.h"
#include "delay.h"

// storing keycode
unsigned char _KEY_H_key;
// get temporary matrix info
unsigned char _KEY_H_temp;

/*
 * Scaning key matrix (key matrix connected to P3, with 4x4 keys)
 * Scan P3 row by row, whenever a pin is pulled down, start scan the
 * row column by column.
 */
int mKeyScan() {
	_KEY_H_key = KEY_NULL;	// if no keys pressed, return KEY_NULL

	// Start scaning S1~S4
	P3 = 0xfe;
    _KEY_H_temp = P3;
    _KEY_H_temp = _KEY_H_temp & 0xf0;	// Check down-pulling in P3.7~P3.4
    if(_KEY_H_temp != 0xf0) {	// Pulling down in P3
      delay(10);				// In case of noise
      if(_KEY_H_temp != 0xf0) {
        _KEY_H_temp = P3;		// Get which pin
        switch(_KEY_H_temp) {
			case 0xee: _KEY_H_key = KEY_SIN; break;	// Pulling down on P3.4(and P3.0)
			case 0xde: _KEY_H_key = KEY_SQR; break;
			case 0xbe: _KEY_H_key = KEY_TRI; break;
			case 0x7e: _KEY_H_key = KEY_SAW; break;
        }
        while(_KEY_H_temp != 0xf0) {	// Wait untill the key is released
			_KEY_H_temp = P3;
			_KEY_H_temp = _KEY_H_temp & 0xf0;
		}
      }
    }
	// Start scaning S5~S7
    P3 = 0xfd;
    _KEY_H_temp = P3;
    _KEY_H_temp = _KEY_H_temp & 0xf0;
    if(_KEY_H_temp != 0xf0) {
		delay(10);
		if(_KEY_H_temp != 0xf0) {
			_KEY_H_temp = P3;
			switch(_KEY_H_temp) {
			case 0xed: _KEY_H_key = KEY_FRE_R_ADD; break;
			case 0xdd: _KEY_H_key = KEY_FRE_A_ADD; break;
			case 0xbd: _KEY_H_key = KEY_AMP_R_ADD; break;
			case 0x7d: _KEY_H_key = KEY_AMP_A_ADD; break;
			} 
			while(_KEY_H_temp != 0xf0) {
				_KEY_H_temp = P3;
				_KEY_H_temp = _KEY_H_temp & 0xf0;
			}
		}
	}
	// Start scaning S8~S12
    P3 = 0xfb;
    _KEY_H_temp = P3;
    _KEY_H_temp = _KEY_H_temp & 0xf0;
    if(_KEY_H_temp != 0xf0) {
		delay(10);
		if(_KEY_H_temp != 0xf0) {
			_KEY_H_temp = P3;
			switch(_KEY_H_temp) {
				case 0xeb: _KEY_H_key = KEY_FRE_R_SUB; break;
				case 0xdb: _KEY_H_key = KEY_FRE_A_SUB; break;
				case 0xbb: _KEY_H_key = KEY_AMP_R_SUB; break;
				case 0x7b: _KEY_H_key = KEY_AMP_A_SUB; break;
			}
			while(_KEY_H_temp != 0xf0) {
				_KEY_H_temp = P3;
				_KEY_H_temp = _KEY_H_temp & 0xf0;
			}
		}
	}
	// Start scaning S13~S15
    P3 = 0xf7;
    _KEY_H_temp = P3;
    _KEY_H_temp = _KEY_H_temp & 0xf0;
    if(_KEY_H_temp != 0xf0) {
		delay(10);
		if(_KEY_H_temp != 0xf0) {
			_KEY_H_temp = P3;
			switch(_KEY_H_temp) {
			case 0xe7: _KEY_H_key = KEY_ONOFF; break;
			case 0xd7: _KEY_H_key = KEY_ONOFF; break;
			case 0xb7: _KEY_H_key = KEY_ONOFF; break;
			case 0x77: _KEY_H_key = KEY_ONOFF; break;
			}
			while(_KEY_H_temp != 0xf0) {
				_KEY_H_temp = P3;
				_KEY_H_temp = _KEY_H_temp & 0xf0;
			}			
		}
    }
	return _KEY_H_key;
}
