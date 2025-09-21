#include "IP_Driver.h"

IP_Driver::IP_Driver(u32 base_address, int size_in_k) {
    base_addr_ = base_address;
    (void)size_in_k; // unused in bare-metal
}

IP_Driver::~IP_Driver() {
    // nothing to clean up in bare-metal
}

void IP_Driver::write(int offset, int data) {
    Xil_Out32(base_addr_ + offset, data);
}

int IP_Driver::read(int offset) {
    return Xil_In32(base_addr_ + offset);
}
