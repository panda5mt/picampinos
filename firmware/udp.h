// 
//  this code is based on https://github.com/kingyoPiyo/RasPico_SHIBAKI_Board
//  this code has copyright and used under licenses below

//  MIT License
//  ------------------  
//  Copyright (c) 2022 kingyo
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.


#ifndef __UDP_H__
#define __UDP_H__

#include <stdint.h>
#include "pico/stdlib.h"

// Buffer size config
#define DEF_UDP_PAYLOAD_SIZE    (1300)

// Ethernet
#define DEF_ETH_DST_MAC         (0xffffffffffff)    // L2 Broadcast
#define DEF_ETH_SRC_MAC         (0x123456789ABC)    // Dummy

// IP Header
#define DEF_IP_ADR_SRC1         (169)
#define DEF_IP_ADR_SRC2         (254)
#define DEF_IP_ADR_SRC3         (201)
#define DEF_IP_ADR_SRC4         (82)

#define DEF_IP_DST_DST1         (169)
#define DEF_IP_DST_DST2         (254)
#define DEF_IP_DST_DST3         (201)
#define DEF_IP_DST_DST4         (216)

// UDP Header
#define DEF_UDP_SRC_PORTNUM     (1024)
#define DEF_UDP_DST_PORTNUM     (1024)
#define DEF_UDP_LEN             (DEF_UDP_PAYLOAD_SIZE + 8)

// -------------------
// Preamble     7
// SFD          1
// Ether        14
// IP Header    20
// UDP Header   8
// UDP Payload  x
// FCS          4
// -------------------
//              x + 54
#define DEF_UDP_BUF_SIZE        (DEF_UDP_PAYLOAD_SIZE + 54)


void udp_init(void);
void __time_critical_func(udp_packet_gen)(uint32_t *buf, uint8_t *udp_payload);

#endif //__UDP_H__

