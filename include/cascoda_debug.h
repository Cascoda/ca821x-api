/*
 * Copyright (c) 2016, Cascoda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CASCODA_DEBUG_H
#define CASCODA_DEBUG_H

#include <stddef.h>

/******************************************************************************/
/****** Function Declarations for cascoda_debug.c                        ******/
/******************************************************************************/
void dpFlush(void *pDeviceRef);
void dp8(uint8_t Out, void *pDeviceRef);
void dp16(uint16_t Out, void *pDeviceRef);
void dp32(uint32_t Out, void *pDeviceRef);
void dps(const uint8_t *pString, void *pDeviceRef);
void dptime(void);
void dparray(const uint8_t *pArray, uint8_t Length, void *pDeviceRef);
void dparrayle(const uint8_t *pArray, uint8_t Length, void *pDeviceRef);
void dpc(uint8_t Out, void *pDeviceRef);
void dpnl(void *pDeviceRef );
void dpDec(uint16_t Out, void *pDeviceRef);
void dp_u8_2h(uint8_t x, void *pDeviceRef);
void dp_u8_3u(uint8_t d, void *pDeviceRef);
void dp_u8_u(uint8_t d, void *pDeviceRef);
void dp_s8_s(int8_t d, void *pDeviceRef);
void dp_u16_5u(uint16_t d, void *pDeviceRef);
void dp_u16_u(uint16_t d, void *pDeviceRef);
void dp_s16_s(int16_t d, void *pDeviceRef);
void dp_u32_10u(uint32_t d, void *pDeviceRef);
void dp_u32_u(uint32_t d, void *pDeviceRef);
void dpslow(const uint8_t *pString, void *pDeviceRef);

extern void (*EVBME_Message)(char *message, size_t len, void *pDeviceRef);

#endif // CASCODA_DEBUG_H
