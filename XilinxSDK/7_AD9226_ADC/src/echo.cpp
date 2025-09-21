/*
 * Copyright (C) 2009 - 2019 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "xil_printf.h"
#include "data_capture.h"

/* Setup AD9226 ADC */
int *captured_data_rf_ = nullptr;
Data_Capture *dc = nullptr;
inline short decode_ad9226(int raw_word, int *otr);

// constants can also be global
const int num_captured_words = 16384;
const int size_in_k = 64;
int otr;

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                    struct pbuf *p, err_t err)
{
    /* do not read the packet if we are not in ESTABLISHED state */
    if (!p) {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    /* indicate that the packet has been received */
    tcp_recved(tpcb, p->len);

    // Capture ADC
    dc->capture_and_read(captured_data_rf_);

    // Decode ADC samples
    short decoded[num_captured_words];
    for (int i = 0; i < num_captured_words; i++)
        decoded[i] = decode_ad9226(captured_data_rf_[i], &otr);

    // Send decoded data in chunks
    int bytes_remaining = num_captured_words * sizeof(short);
    int offset = 0;
    const int CHUNK_SIZE = 1024; // adjust to your TCP_SND_BUF

    while (bytes_remaining > 0) {
        u16_t to_send = bytes_remaining > CHUNK_SIZE ? CHUNK_SIZE : bytes_remaining;

        if (tcp_sndbuf(tpcb) < to_send) {
            // Not enough room, try again later
            break;
        }

        err = tcp_write(tpcb, ((char*)decoded) + offset, to_send, TCP_WRITE_FLAG_COPY);
        if (err != ERR_OK) {
            xil_printf("tcp_write failed! err=%d\n\r", err);
            break;
        }

        offset += to_send;
        bytes_remaining -= to_send;

        // Push the data to the network immediately
        tcp_output(tpcb);
    }

    /* free the received pbuf */
    pbuf_free(p);

    return ERR_OK;
}


err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

    xil_printf("Start AD9226 ADC Data Capture\n");

    captured_data_rf_ = new int[num_captured_words];

    dc = new Data_Capture(
        XPAR_AXIS_CAPTURE_0_BASEADDR,
        size_in_k,
        num_captured_words);

	return 0;
}

int close_application() {
	delete[] captured_data_rf_;
    delete dc;
    return 0;
}

inline short decode_ad9226(int raw_word, int *otr) {
    // Extract ADC sample from bits [31:20]
    int sample = (raw_word >> 20) & 0xFFF;

    // Sign extend to 16-bit
//    if (sample & 0x800) {
//        sample |= 0xFFFFF000;
//    }

    // Extract OTR (bit 16)
    *otr = (raw_word >> 16) & 0x1;

    return (short)sample;
}
