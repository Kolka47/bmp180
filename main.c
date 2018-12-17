/*
 * main.c
 *
 *  Created on: 15 gru 2018
 *      Author: Wojciech Kolisz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "bmp180/bmp180.h"
#include "pcd8544/pcd8544.h"



int main()
{



	DDRD |= (1<<PD5);
	PORTD |= (1<<PD5);

	Lcd_Init();
	Lcd_Clr();

	bmp180_init();


    while(1)
    {

    	bmp180_ut();
    	bmp180_up();

    	Lcd_Locate(0,0);
		Lcd_Int(bmp180_temp()/10, DEC );
	    Lcd_Str(".");
		Lcd_Int(bmp180_temp()%10, DEC );
		Lcd_Char(127);
	    Lcd_Str("C");


	    Lcd_Locate(0,5);
	    Lcd_Int(bmp180_press()/100, DEC );
	    Lcd_Str(".");
	    if(bmp180_press()%100 < 10) Lcd_Char('0');
	    Lcd_Int(bmp180_press()%100, DEC);
	    Lcd_Str("hPa");


		Lcd_Upd();
		_delay_ms(1000);
		Lcd_Clr();


    }


}
