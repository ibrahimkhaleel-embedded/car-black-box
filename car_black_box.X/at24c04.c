
#include "main.h"
#include "at24c04.h"
#include "i2c.h"
#include <xc.h>

/* 
 * at24c04 Slave address
 * A0  -  Write Mode
 * A1  -  Read Mode
 */


void write_at24c04(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
}

unsigned char read_at24c04(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}