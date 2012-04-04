#ifndef M_I2C_H
#define M_I2C_H

#include "mbed.h"


class m_I2C {

    protected:
        char buf[10];
        int status;
        int addressR;
        int addressW;
        I2C i2c;
    
    public:
        m_I2C();
        m_I2C(int i, int addrR, int addrW,PinName a, PinName b);
        ~m_I2C();
        
        int bus_init(int channel, int addrR, int addrW);
        int get_status();
        int write_address(unsigned char reg);
        int write_byte(unsigned char reg, unsigned char data);
        int write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask);
        int read_current_byte(unsigned char &data);
        int read_byte(unsigned char reg, unsigned char &data);
};


m_I2C::m_I2C(int channel, int addrR, int addrW, PinName a, PinName b) : i2c(a,b) {
    bus_init(channel,addrR,addrW);
}

m_I2C::~m_I2C() {

}

int m_I2C::bus_init(int channel, int addrR, int addrW) {
    addressR = addrR;
    addressW = addrW;
    i2c.frequency(100000);
    status = 0;
    return 0;
}

int m_I2C::write_address(unsigned char reg) {
    buf[0] = reg;
    return i2c.write(addressW,buf,1);
}
 
int m_I2C::write_byte(unsigned char reg, unsigned char data) {
    buf[0] = reg;
    buf[1] = data;
    return i2c.write(addressW,buf,2);
}
  
int m_I2C::write_masked_byte(unsigned char reg, unsigned char data, unsigned char mask) {
    return 0;
}

int m_I2C::read_current_byte(unsigned char &data) {
    int status = i2c.read(addressR,buf,1);
    data = buf[0];
    return status;
}

int m_I2C::read_byte(unsigned char reg, unsigned char &data) {
    int status = write_address(reg);
    return read_current_byte(data);
}

#endif