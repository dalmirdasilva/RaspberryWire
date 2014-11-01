
/**
 * This is a siple Wire library to Raspberry. 
 * It doesn't use the specific i2c module (i2c_dev or i2c_bcm2708)
 * it maps the memory (the BSC0 chunk) into the virtual memory space 
 * and handles directly the register.
 * 
 * Thanks to this blog: http://www.susa.net/wordpress/2012/06/raspberry-pi-pcf8563-real-time-clock-rtc/
 */
 
#ifndef __RASPBERRY_WIRE_USER_SPACE_H__
#define __RASPBERRY_WIRE_USER_SPACE_H__ 1

#include <Wire.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

class WireUserSpace : public Wire {
    
    /**
     * Channel (0 or 1)
     */
	unsigned char channel;

	/**
	 * File descriptor.
	 */
    int fd;

public:

    WireUserSpace(unsigned char channel);

    /**
     * Initiate the library. (Only as a master)
     * This should normally be called only once.
     * It maps the BSC0 (0x7E20_5000) registers.
     */
    virtual void begin();

    /**
     * Unmap the BSC0 registers.
     */
    virtual void stop();
    
    /**
     * Begin a transmission to the I2C slave device with the given 
     * address. Subsequently, queue bytes for transmission with the 
     * write() function and transmit them by calling endTransmission().
     * 
     * @param address               The device address.
     */
    virtual void beginTransmission(int address);
    
    /**
     * Ends a transmission to a slave device that was begun by 
     * beginTransmission() and transmits the bytes that were queued by 
     * write().
     * 
     * @return                      Nothing for now.
     */
    virtual unsigned char endTransmission();
    
    /**
     * Used to request bytes from a slave device. The bytes may then be 
     * retrieved with the available() and read() functions.
     * 
     * @param address               The slave address.
     * @param len                   The length of data. Need be <= 16 
     *                              due the FIFO limits.
     */
    virtual unsigned char requestFrom(int address, unsigned int len);
    
    /**
     * Queues a single byte for transmission to slave device (in-between
     * calls to beginTransmission() and endTransmission()).
     * 
     * @param b                     The byte to be queued.
     * @return                      1 if the byte was accepted or 0 if
     *                              the internal FIFO does not accepted.
     */
    virtual unsigned int write(unsigned char b);
    
    /**
     * Queues bytes for transmission to slave device (in-between calls 
     * to beginTransmission() and endTransmission()).
     * 
     * @param buf                   The bytes to be queued.
     * @param len                   The number of byte to be queued.
     * @return                      The number of accepted bytes.
     */
    virtual unsigned int write(const unsigned char* buf, unsigned int len);
    
    /**
     * Returns 1 if there is one or more bytes to be read.
     * 
     * @return                      0 if there is no bytes to be read in
     *                              the internal FIFO.
     */
    virtual int available();
    
    /**
     * Reads a byte that was transmitted from a slave device to a master
     * after a call to requestFrom()
     *
     * @return                      The byte read.
     */
    virtual int read();
    
    /**
     * For now, does nothing.
     */
    virtual void flush();
};

extern WireUserSpace WireUS0;
extern WireUserSpace WireUS1;

#endif /* __RASPBERRY_WIRE_USER_SPACE_H__ */
