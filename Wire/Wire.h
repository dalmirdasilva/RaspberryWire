
/**
 * This is a siple Wire library to Raspberry interface.
 */
 
#ifndef __RASPBERRY_WIRE_H__
#define __RASPBERRY_WIRE_H__ 1

class Wire {
    
public:

	/**
	 * Destructor.
	 */
    virtual ~Wire() {
    }

    /**
     * Initiate the library. (Only as a master)
     * This should normally be called only once.
     * It maps the BSC0 (0x7E20_5000) registers.
     */
    virtual void begin() = 0;

    /**
     * Unmap the BSC0 registers.
     */
    virtual void stop() = 0;
    
    /**
     * Begin a transmission to the I2C slave device with the given 
     * address. Subsequently, queue bytes for transmission with the 
     * write() function and transmit them by calling endTransmission().
     * 
     * @param address               The device address.
     */
    virtual void beginTransmission(int address) = 0;
    
    /**
     * Begin a transmission to the I2C slave device with the given 
     * address. Subsequently, queue bytes for transmission with the 
     * write() function and transmit them by calling endTransmission().
     * 
     * @param address               The device address.
     */
    //void beginTransmission(unsigned char address);
    
    /**
     * Ends a transmission to a slave device that was begun by 
     * beginTransmission() and transmits the bytes that were queued by 
     * write().
     * 
     * @return                      Nothing for now.
     */
    virtual unsigned char endTransmission() = 0;
    
    /**
     * Used to request bytes from a slave device. The bytes may then be 
     * retrieved with the available() and read() functions.
     * 
     * @param address               The slave address.
     * @param len                   The length of data. Need be <= 16 
     *                              due the FIFO limits.
     */
    //unsigned char requestFrom(unsigned char address, unsigned char len);
    
    /**
     * Used to request bytes from a slave device. The bytes may then be 
     * retrieved with the available() and read() functions.
     * 
     * @param address               The slave address.
     * @param len                   The length of data. Need be <= 16 
     *                              due the FIFO limits.
     */
    virtual unsigned char requestFrom(int address, unsigned int len) = 0;
    
    /**
     * Queues a single byte for transmission to slave device (in-between
     * calls to beginTransmission() and endTransmission()).
     * 
     * @param b                     The byte to be queued.
     * @return                      1 if the byte was accepted or 0 if
     *                              the internal FIFO does not accepted.
     */
    virtual unsigned int write(unsigned char b) = 0;
    
    /**
     * Queues bytes for transmission to slave device (in-between calls 
     * to beginTransmission() and endTransmission()).
     * 
     * @param buf                   The bytes to be queued.
     * @param len                   The number of byte to be queued.
     * @return                      The number of accepted bytes.
     */
    virtual unsigned int write(const unsigned char* buf, unsigned int len) = 0;
    
    /**
     * Returns 1 if there is one or more bytes to be read.
     * 
     * @return                      0 if there is no bytes to be read in
     *                              the internal FIFO.
     */
    virtual int available() = 0;
    
    /**
     * Reads a byte that was transmitted from a slave device to a master
     * after a call to requestFrom()
     *
     * @return                      The byte read.
     */
    virtual int read() = 0;
    
    /**
     * For now, does nothing.
     */
    virtual void flush() = 0;
};

#endif /* __RASPBERRY_WIRE_H__ */
