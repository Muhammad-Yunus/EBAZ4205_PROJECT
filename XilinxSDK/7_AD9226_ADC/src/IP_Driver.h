#ifndef IP_DRIVER_H
#define IP_DRIVER_H

#include "xil_io.h"
#include "xparameters.h"

class IP_Driver {
public:
    IP_Driver(u32 base_address, int size_in_k);
    ~IP_Driver();

    void write(int offset, int data);
    int read(int offset);

private:
    u32 base_addr_;
};

#endif
