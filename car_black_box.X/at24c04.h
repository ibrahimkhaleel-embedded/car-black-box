#ifndef at24c04_H
#define at24c04_H

#define SLAVE_READ		0xA1
#define SLAVE_WRITE		0xA0




void write_at24c04(unsigned char address1,  unsigned char data);
unsigned char read_at24c04(unsigned char address1);
//void init_at24c04(void);

#endif