
#include "WireKernelSpace.h"

WireKernelSpace::WireKernelSpace(unsigned char channel) {
    this->channel = (channel & 0x01);
    this->txSize = 0;
}

void WireKernelSpace::begin() {
    bsc.address = (this->channel == 0) ? BSC0_ADDRESS : BSC1_ADDRESS;
    Bcm2835::mapPeripheral(&bsc);
}

void WireKernelSpace::stop() {
    Bcm2835::unmapPeripheral(&bsc);
}

void WireKernelSpace::beginTransmission(int address) {
    BSC_A = (address & 0x7ff);
    BSC_DLEN = 0;
    txSize = 0;
}

unsigned char WireKernelSpace::endTransmission(void) {
    BSC_DLEN = txSize;
    BSC_S = CLEAR_STATUS;
    BSC_C = START_WRITE;
    waitDone();
    return txSize;
}

unsigned char WireKernelSpace::requestFrom(int address, unsigned int len) {
    BSC_A = (address & 0x7ff);
    BSC_DLEN = len;
    BSC_S = CLEAR_STATUS;
    BSC_C = START_READ;
    waitDone();
    return 0;
}

unsigned int WireKernelSpace::write(unsigned char b) {
    if (BSC_S & BSC_S_TXD) {
        txSize++;
        BSC_FIFO = b;
        return 1;
    }
    return 0;
}

unsigned int WireKernelSpace::write(const unsigned char* buf, unsigned int len) {
	unsigned int i;
    for (i = 0; i < len; i++) {
        if (!write(buf[i])) {
            break;
        }
    }
    return i;
}

int WireKernelSpace::available() {
    return (bool) (BSC_S & BSC_S_RXD);
}

int WireKernelSpace::read() {
	dumpStatus();
	unsigned char b = BSC_FIFO;
	printf("read: %x\n", b);
    return b;
}

void WireKernelSpace::flush() {
}

bool WireKernelSpace::isDone() {
    return (bool) (BSC_S & BSC_S_DONE);
}

void WireKernelSpace::waitDone() {
    int timeout = 60;
    while(!isDone() && --timeout) {
        usleep(1000);
    }
    if(timeout == 0) {
        perror("#waitDone: Timeout! Something went wrong.\n");
    }
}

void WireKernelSpace::dumpStatus() {
    unsigned int s = BSC_S;
    printf("BSC_S: ERR=%d  RXF=%d  TXE=%d  RXD=%d  TXD=%d  RXR=%d  TXW=%d  DONE=%d  TA=%d\n",
        (s & BSC_S_ERR) != 0,
        (s & BSC_S_RXF) != 0,
        (s & BSC_S_TXE) != 0,
        (s & BSC_S_RXD) != 0,
        (s & BSC_S_TXD) != 0,
        (s & BSC_S_RXR) != 0,
        (s & BSC_S_TXW) != 0,
        (s & BSC_S_DONE) != 0,
        (s & BSC_S_TA) != 0);
}

WireKernelSpace WireKS0(0);
WireKernelSpace WireKS1(1);
