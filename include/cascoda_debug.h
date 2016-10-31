/*
 * @file cascoda_debug.h
 * @brief Debug printing functions for Cascoda API code and applications.
 *
 * Copyright (C) 2016  Cascoda, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
