/*
 * bmp180.h
 *
 *  Created on: 15 gru 2018
 *      Author: Wojciech Kolisz
 */

#ifndef BMP180_BMP180_H_
#define BMP180_BMP180_H_

#define ACK 1
#define NACK 0
#define SLA 0xEE	//bmp180 address



void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t byte);
uint8_t I2C_read(uint8_t ack);
void I2C_write_buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf);
void I2C_read_buf(uint8_t sla, uint8_t adr, uint8_t len, uint8_t *buf);



#endif /* BMP180_BMP180_H_ */
