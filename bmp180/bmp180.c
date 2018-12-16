/*
 * bmp180.c
 *
 *  Created on: 15 gru 2018
 *      Author: Wojciech Kolisz
 */

#include <avr/io.h>

#include "bmp180.h"




void I2C_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	while (!(TWCR & (1<<TWINT)));
}

void I2C_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while ((TWCR & (1<<TWSTO)));
}

void I2C_write(uint8_t byte)
{
	TWDR = byte;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

uint8_t I2C_read(uint8_t ack)
{
	TWCR = (1<<TWINT) | (ack<<TWEA) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void I2C_write_buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf)
{
	I2C_start();
	I2C_write(SLA);
	I2C_write(adr);
	while (len--); I2C_write(*buf++);
	I2C_stop();
}

void I2C_read_buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf)
{
	I2C_start();
	I2C_write(sla);
	I2C_write(adr);
	I2C_start();
	I2C_write(sla+1);
	while(len--) *buf++ = I2C_read(len ? ACK : NACK);
	I2C_stop();
}

