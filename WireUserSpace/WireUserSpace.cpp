
#include "WireUserSpace.h"

WireUserSpace::WireUserSpace(unsigned char channel) {
	this->channel = channel;
	this->fd = 0;
}

void WireUserSpace::begin() {
	char f[11] = "/dev/i2c-0";
	f[9] = '0' + channel;
	fd = open(f, O_RDWR);
	if (fd < 0) {
		perror("Cannot open i2c bus.");
		exit(1);
	}
}

void WireUserSpace::stop() {
	close(fd);
}

void WireUserSpace::beginTransmission(int address) {
	if (ioctl(fd, I2C_SLAVE, address) < 0) {
		perror("Cannot set i2c address.");
		exit(1);
	}
}

unsigned char WireUserSpace::endTransmission(void) {
	return 0;
}

unsigned char WireUserSpace::requestFrom(int address, unsigned int len) {
	if (ioctl(fd, I2C_SLAVE, address) < 0) {
		perror("Cannot set i2c address.");
		exit(1);
	}
	return len;
}

unsigned int WireUserSpace::write(unsigned char b) {
	return write(&b, 1);
}

unsigned int WireUserSpace::write(const unsigned char* buf, unsigned int len) {
	return ::write(fd, buf, (int)len);
}

int WireUserSpace::available() {
	return 1;
}

int WireUserSpace::read() {
	char buf[1];
	if (::read(fd, buf, 1) != 1) {
		perror("Cannot read i2c.");
		exit(1);
	}
	return buf[0];
}


void WireUserSpace::flush() {
}

WireUserSpace WireUS0(0);
WireUserSpace WireUS1(1);
