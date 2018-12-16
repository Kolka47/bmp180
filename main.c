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


    long AC1, AC2, AC3, B1, B2, MB, MC, MD;
    unsigned short AC4, AC5, AC6;
    long T, UT, X1, X2, X3, B3, B4, B5, B6, B7, p;
    short oss;
    long UP;

    oss = 0;


    uint8_t bufor[16];
    uint8_t buf2[1], buf3[1];
    buf2[0] = 0x2E;
    buf3[0] = 0x34 +(oss<<6) ;





    uint8_t T_int, T_dec, P_int, P_dec;

	//odczyt AC1	0xAA 0xAB
	I2C_read_buf(SLA, 0xAA, 2, bufor);
	AC1 = (bufor[0] << 8) + bufor[1];

	//odczyt AC2	0xAC 0xAD
	I2C_read_buf(SLA, 0xAC, 2, bufor);
	AC2 = (bufor[0] << 8) + bufor[1];

	//odczyt AC3	0xAE 0xAF
	I2C_read_buf(SLA, 0xAE, 2, bufor);
	AC3 = (bufor[0] << 8) + bufor[1];

	//odczyt AC4	0xB0 0xB1
	I2C_read_buf(SLA, 0xB0, 2, bufor);
	AC4 = (bufor[0] << 8) + bufor[1];

	//odczyt AC5	0xB2 0xB3
	I2C_read_buf(SLA, 0xB2, 2, bufor);
	AC5 = (bufor[0] << 8) + bufor[1];

	//odczyt AC6	0xB4 0xB5
	I2C_read_buf(SLA, 0xB4, 2, bufor);
	AC6 = (bufor[0] << 8) + bufor[1];

	//odczyt B1		0xB6 0xB7
	I2C_read_buf(SLA, 0xB6, 2, bufor);
	B1 = (bufor[0] << 8) + bufor[1];

	//odczyt B2		0xB8 0xB9
	I2C_read_buf(SLA, 0xB8, 2, bufor);
	B2 = (bufor[0] << 8) + bufor[1];

	//odczyt MB		0xBA oxBB
	I2C_read_buf(SLA, 0xBA, 2, bufor);
	MB = (bufor[0] << 8) + bufor[1];

	//odczyt MC		0xBC 0xBD
	I2C_read_buf(SLA, 0xBC, 2, bufor);
	MC = (bufor[0] << 8) + bufor[1];

	//odczyt MD		0xBE 0xBF
	I2C_read_buf(SLA, 0xBE, 2, bufor);
	MD = (bufor[0] << 8) + bufor[1];

    while(1)
    {





    	// UT read uncompensated temperature value
    	I2C_write_buf(SLA, 0xF4, 1,buf2 );
    	_delay_ms(5);
    	I2C_read_buf(SLA, 0xF6, 2, bufor);
    	UT = (bufor[0] << 8) + bufor[1];

    	// UP read uncompensated pressure value
    	I2C_write_buf(SLA, 0xF4, 1, buf3);
    	_delay_ms(30);
    	I2C_read_buf(SLA, 0xF6, 3, bufor);
    	PCD_Locate(1,3);
	    PCD_Int(FONT_1X, bufor[0] );
    	PCD_Locate(1,4);
	    PCD_Int(FONT_1X, bufor[1] );
    	PCD_Locate(1,5);
	    PCD_Int(FONT_1X, bufor[2] );

    	UP = (((unsigned long )bufor[0] << 16) + (bufor[1] << 8) + bufor[2])>>(8-oss);


    	// calculate true temperature
    	X1 = (UT - AC6)*AC5/32768;
    	X2 = MC * 2048/(X1 + MD);
    	B5 = X1+X2;
    	T = (B5 + 8)/16;


    	//calculate true pressure
    	B6 = B5 - 4000;
    	X1 = (B2 * (B6*B6/4096))/2048;
    	X2 = AC2 * B6 / 2048;
    	X3 = X1 + X2;
    	B3 = (((AC1*4+X3)<<oss)+2)/4;
    	X1 = AC3*B6/8192;
    	X2 = (B1*(B6*B6/4096))/65536;
    	X3 = ((X1+X2)+2)/4;
    	B4 = AC4*(unsigned long)(X3 + 32768)/32768;
    	B7 = ((unsigned long)UP - B3)*(50000>>oss);
    	if(B7 < 0x80000000) p = (B7*2)/B4;
    	else p = (B7/B4)*2;
    	X1 = (p/256)*(p/256);
    	X1 = (X1*3038)/65536;
    	X2 = (-7357*p)/65536;
    	p = p+(X1+X2+3791)/16;



    	T_int = T/10;
    	T_dec = T % 10;

    	P_int = p/100;
    	P_dec = p%100;

    	PCD_Locate(1,2);
		PCD_Int(FONT_2X,T_int);
	    PCD_FStr(FONT_2X,(unsigned char*)PSTR("."));
		PCD_Int(FONT_2X,T_dec);
		//PCD_Chr(FONT_2X, 123);
	    PCD_FStr(FONT_2X,(unsigned char*)PSTR("{C"));


	    PCD_Locate(1,6);
	    PCD_Int(FONT_1X, P_int );
	    PCD_FStr(FONT_1X,(unsigned char*)PSTR("."));
	    PCD_Int(FONT_1X, P_dec );
	    PCD_FStr(FONT_1X,(unsigned char*)PSTR("hPa"));








		PCD_Upd();


		_delay_ms(1000);
		PCD_Clr();

    }

}
