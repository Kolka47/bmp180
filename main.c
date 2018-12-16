/*
 * main.c
 *
 *  Created on: 15 gru 2018
 *      Author: Wojciech Kolisz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "bmp180/bmp180.h"
#include "pcd8544/pcd8544.h"

int main()
{
	DDRD |= (1<<PD5);
	PORTD |= (1<<PD5);

    PCD_On();
	PCD_Ini();  // inicjacja LCD
    PCD_Contr(0xC3);  // kontrast
    PCD_Clr();




    uint8_t bufor[16];
    uint8_t buf2[1];
    buf2[0] = 0x2E;


    long AC5, AC6, MC, MD, UT;
    long X1, X2, B5;
    long T;

    uint8_t T_int, T_dec;

	//odczyt AC5
	I2C_read_buf(SLA, 0xB2, 2, bufor);
	AC5 = (bufor[0] << 8) + bufor[1];

	//odczyt AC6
	I2C_read_buf(SLA, 0xB4, 2, bufor);
	AC6 = (bufor[0] << 8) + bufor[1];

	//odczyt MC
	I2C_read_buf(SLA, 0xBC, 2, bufor);
	MC = (bufor[0] << 8) + bufor[1];

	//odczyt MD
	I2C_read_buf(SLA, 0xBE, 2, bufor);
	MD = (bufor[0] << 8) + bufor[1];

    while(1)
    {





    	//odczyt UT
    	I2C_write_buf(SLA, 0xF4, 1,buf2 );
    	_delay_ms(5);
    	I2C_read_buf(SLA, 0xF6, 2, bufor);
    	UT = (bufor[0] << 8) + bufor[1];


    	//obliczanie temperatury
    	X1 = (UT - AC6)*AC5/32768;

    	X2 = MC * 2048/(X1 + MD);
    	B5 = X1+X2;
    	T = (B5 + 8)/16;

    	T_int = T/10;
    	T_dec = T % 10;

    	PCD_Locate(1,2);
		PCD_Int(FONT_2X,T_int);
	    PCD_FStr(FONT_2X,(unsigned char*)PSTR("."));
		PCD_Int(FONT_2X,T_dec);
	    PCD_FStr(FONT_2X,(unsigned char*)PSTR("C"));








		PCD_Upd();


		_delay_ms(1000);
		PCD_Clr();

    }

}
