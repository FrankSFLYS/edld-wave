#include "key.h"
#include "delay.h"

unsigned char _KEY_H_key;
unsigned char _KEY_H_temp;
// int* InSettingsMode;

// void keyInit(int* settings) {
// //	Init_EXTI();
// 	InSettingsMode = settings;
// 	_KEY_H_K1 = 1;
// //	P3 = 0x0F;
// }

// int keyScan() {
// 	if(_KEY_H_K1 == 0) {
// 		while(_KEY_H_K1 == 0);
// 		return 1;
// 	} else {
// 		return 0;
// 	}
// }

int mKeyScan() {
	_KEY_H_key = KEY_NULL;
	P3=0xfe;
    _KEY_H_temp=P3;
    _KEY_H_temp=_KEY_H_temp&0xf0;
    if(_KEY_H_temp!=0xf0) {
      delay(10);
      if(_KEY_H_temp!=0xf0) {
        _KEY_H_temp=P3;
        switch(_KEY_H_temp) {
			case 0xee: _KEY_H_key = KEY_SIN; break;
			case 0xde: _KEY_H_key = KEY_SQR; break;
			case 0xbe: _KEY_H_key = KEY_TRI; break;
			case 0x7e: _KEY_H_key = KEY_SAW; break;
        }
        while(_KEY_H_temp!=0xf0) {
			_KEY_H_temp=P3;
			_KEY_H_temp=_KEY_H_temp&0xf0;
		}
      }
    }
    P3=0xfd;
    _KEY_H_temp=P3;
    _KEY_H_temp=_KEY_H_temp&0xf0;
    if(_KEY_H_temp!=0xf0) {
		delay(10);
		if(_KEY_H_temp!=0xf0) {
			_KEY_H_temp=P3;
			switch(_KEY_H_temp) {
			case 0xed: _KEY_H_key = KEY_FRE_R_ADD; break;
			case 0xdd: _KEY_H_key = KEY_FRE_A_ADD; break;
			case 0xbd: _KEY_H_key = KEY_AMP_R_ADD; break;
			case 0x7d: _KEY_H_key = KEY_AMP_A_ADD; break;
			} 
			while(_KEY_H_temp!=0xf0) {
				_KEY_H_temp=P3;
				_KEY_H_temp=_KEY_H_temp&0xf0;
			}
		}
	}
    P3=0xfb;
    _KEY_H_temp=P3;
    _KEY_H_temp=_KEY_H_temp&0xf0;
    if(_KEY_H_temp!=0xf0) {
		delay(10);
		if(_KEY_H_temp!=0xf0) {
			_KEY_H_temp=P3;
			switch(_KEY_H_temp) {
				case 0xeb: _KEY_H_key = KEY_FRE_R_SUB; break;
				case 0xdb: _KEY_H_key = KEY_FRE_A_SUB; break;
				case 0xbb: _KEY_H_key = KEY_AMP_R_SUB; break;
				case 0x7b: _KEY_H_key = KEY_AMP_A_SUB; break;
			}
			while(_KEY_H_temp!=0xf0) {
				_KEY_H_temp=P3;
				_KEY_H_temp=_KEY_H_temp&0xf0;
			}
		}
	}
    P3=0xf7;
    _KEY_H_temp=P3;
    _KEY_H_temp=_KEY_H_temp&0xf0;
    if(_KEY_H_temp!=0xf0) {
		delay(10);
		if(_KEY_H_temp!=0xf0) {
			_KEY_H_temp=P3;
			switch(_KEY_H_temp) {
			case 0xe7: _KEY_H_key = KEY_ONOFF; break;
			case 0xd7: _KEY_H_key = KEY_ONOFF; break;
			case 0xb7: _KEY_H_key = KEY_ONOFF; break;
			case 0x77: _KEY_H_key = KEY_ONOFF; break;
			}
			while(_KEY_H_temp!=0xf0) {
				_KEY_H_temp=P3;
				_KEY_H_temp=_KEY_H_temp&0xf0;
			}			
		}
    }
	return _KEY_H_key;
}

// void EnterSettingsMode() interrupt 0 {
// 	EA = 0;
// 	*InSettingsMode = 1;
// }