
/**
 * This is a siple Wire library to Raspberry. 
 * It doesn't use the specific i2c module (i2c_dev or i2c_bcm2708)
 * it maps the memory (the BSC0 chunk) into the virtual memory space 
 * and handles directly the register.
 * 
 * Thanks to this blog: http://www.susa.net/wordpress/2012/06/raspberry-pi-pcf8563-real-time-clock-rtc/
 */
 
#ifndef __RASPBERRY_WIRE_H__
#define __RASPBERRY_WIRE_H__ 1

#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#include <Bcm2835.h>

#define BSC0_ADDRESS    0x205000
#define BSC1_ADDRESS    0x804000

#define BSC_C          *((unsigned int *)(bsc.mem) + 0x00)
#define BSC_S          *((unsigned int *)(bsc.mem) + 0x01)
#define BSC_DLEN       *((unsigned int *)(bsc.mem) + 0x02)
#define BSC_A          *((unsigned int *)(bsc.mem) + 0x03)
#define BSC_FIFO       *((unsigned int *)(bsc.mem) + 0x04)

#define BSC_C_I2CEN     (0x01 << 15)
#define BSC_C_INTR      (0x01 << 10)
#define BSC_C_INTT      (0x01 << 9)
#define BSC_C_INTD      (0x01 << 8)
#define BSC_C_ST        (0x01 << 7)
#define BSC_C_CLEAR     (0x01 << 4)
#define BSC_C_READ      (0x01 << 0)

#define START_READ      BSC_C_I2CEN | BSC_C_ST | BSC_C_CLEAR | BSC_C_READ
#define START_WRITE     BSC_C_I2CEN | BSC_C_ST

#define BSC_S_CLKT      (0x01 << 9)
#define BSC_S_ERR       (0x01 << 8)
#define BSC_S_RXF       (0x01 << 7)
#define BSC_S_TXE       (0x01 << 6)
#define BSC_S_RXD       (0x01 << 5)
#define BSC_S_TXD       (0x01 << 4)
#define BSC_S_RXR       (0x01 << 3)
#define BSC_S_TXW       (0x01 << 2)
#define BSC_S_DONE      (0x01 << 1)
#define BSC_S_TA        (0x01 << 0)

#define CLEAR_STATUS    BSC_S_CLKT | BSC_S_ERR | BSC_S_DONE

class WireRPi {
    
    Bcm2835::Peripheral bsc;
    
    /**
     * This cannot be bigger then 16
     */
    int txSize;
    
    /**
     * BSC channel (0 or 1)
     */
    unsigned char channel;

public:

    WireRPi(unsigned char channel);

    /**
     * Initiate the library. (Only as a master)
     * This should normally be called only once.
     * It maps the BSC0 (0x7E20_5000) registers.
     */
    void begin();

    /**
     * Unmap the BSC0 registers.
     */
    void stop();
    
    /**
     * Begin a transmission to the I2C slave device with the given 
     * address. Subsequently, queue bytes for transmission with the 
     * write() function and transmit them by calling endTransmission().
     * 
     * @param address               The device address.
     */
    void beginTransmission(int address);
    
    /**
     * Begin a transmission to the I2C slave device with the given 
     * address. Subsequently, queue bytes for transmission with the 
     * write() function and transmit them by calling endTransmission().
     * 
     * @param address               The device address.
     */
    void beginTransmission(unsigned char address) {
        beginTransmission((int) address);
    }
    
    /**
     * Ends a transmission to a slave device that was begun by 
     * beginTransmission() and transmits the bytes that were queued by 
     * write().
     * 
     * @return                      Nothing for now.
     */
    unsigned char endTransmission();
    
    /**
     * Used to request bytes from a slave device. The bytes may then be 
     * retrieved with the available() and read() functions.
     * 
     * @param address               The slave address.
     * @param len                   The length of data. Need be <= 16 
     *                              due the FIFO limits.
     */
    unsigned char requestFrom(unsigned char address, unsigned char len) {
        return requestFrom((int) address, (int) len);
    }
    
    /**
     * Used to request bytes from a slave device. The bytes may then be 
     * retrieved with the available() and read() functions.
     * 
     * @param address               The slave address.
     * @param len                   The length of data. Need be <= 16 
     *                              due the FIFO limits.
     */
    unsigned char requestFrom(int address, unsigned int len);
    
    /**
     * Queues a single byte for transmission to slave device (in-between
     * calls to beginTransmission() and endTransmission()).
     * 
     * @param b                     The byte to be queued.
     * @return                      1 if the byte was accepted or 0 if
     *                              the internal FIFO does not accepted.
     */
    unsigned int write(unsigned char b);
    
    /**
     * Queues bytes for transmission to slave device (in-between calls 
     * to beginTransmission() and endTransmission()).
     * 
     * @param buf                   The bytes to be queued.
     * @param len                   The number of byte to be queued.
     * @return                      The number of accepted bytes.
     */
    unsigned int write(const unsigned char* buf, unsigned int len);
    
    /**
     * Returns 1 if there is one or more bytes to be read.
     * 
     * @return                      0 if there is no bytes to be read in
     *                              the internal FIFO.
     */
    int available();
    
    /**
     * Reads a byte that was transmitted from a slave device to a master
     * after a call to requestFrom()
     *
     * @return                      The byte read.
     */
    int read();
    
    /**
     * For now, does nothing.
     */
	void flush();
    
    /**
     * Prints the status register.
     */
    void dumpStatus();
    
private:

    /**
     * Checks if the transmission in complete.
     */
    bool isDone();

    /**
     * Waits for the transmission to be complete.
     */
    void waitDone();
};

extern WireRPi Wire0;
extern WireRPi Wire1;

#endif /* __RASPBERRY_WIRE_H__ */
