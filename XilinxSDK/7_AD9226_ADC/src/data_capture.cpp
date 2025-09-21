#include "IP_Driver.h"
#include "data_capture.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

Data_Capture::Data_Capture(int base_address, int size_in_k, int num_captured_words)
    : timeout_max_(10000000) {
    data_capture_core_ = new IP_Driver(base_address, size_in_k);
    num_captured_words_ = num_captured_words;
    data_array_int2double_ = new int[num_captured_words_];
}

Data_Capture::~Data_Capture() {
    delete[] data_array_int2double_;
    delete data_capture_core_;
}

void Data_Capture::capture() {
    data_capture_core_->write(0x0, 0x00000002);
    data_capture_core_->write(0x0, 0x00000000);
    data_capture_core_->write(0x0, 0x00000001);
}

int Data_Capture::read(int *data_array) {
    if (data_capture_core_->read(0x04) == 1)
        return -2;

    int check_address;
    for (int i=0; i < num_captured_words_; i++) {
        timeout_counter_ = 0;
        data_capture_core_->write(0x14, i);
        do {
            check_address = data_capture_core_->read(0x18);
            timeout_counter_++;
        } while ((check_address != i) && (timeout_counter_ < 1000));

        if (timeout_counter_ == 1000)
            return -1;

        data_array[i] = data_capture_core_->read(0x10);
    }
    return 0;
}

int Data_Capture::capture_and_read(int *data_array) {
    capture();
    int module_busy;
    timeout_counter_ = 0;

    do {
        module_busy = data_capture_core_->read(0x04);
        timeout_counter_++;
    } while ((module_busy == 1) && (timeout_counter_ < timeout_max_));

    if (timeout_counter_ == timeout_max_)
        return -3;

    return read(data_array);
}

int Data_Capture::capture_and_read(double *data_array) {
    int err_code = capture_and_read(data_array_int2double_);
    for (int i=0; i < num_captured_words_; i++)
        data_array[i] = (double)data_array_int2double_[i];
    return err_code;
}

int Data_Capture::capture_and_read(double *data_array, int size) {
    int err_code = capture_and_read(data_array_int2double_);
    if (num_captured_words_ < size) {
    	printf("ERROR: array size larger than captured words");
        exit(1);
    }
    for (int i=0; i < size; i++)
        data_array[i] = (double)data_array_int2double_[i];
    return err_code;
}
