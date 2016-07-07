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


#ifndef CASCODA_EVBME_H
#define CASCODA_EVBME_H

#include <stdint.h>

/******************************************************************************/
/****** EVBME Command ID Codes                                           ******/
/******************************************************************************/
#define EVBME_SET_REQUEST             (0x5F)
#define EVBME_GUI_CONNECTED           (0x81)
#define EVBME_GUI_DISCONNECTED        (0x82)
#define EVBME_MESSAGE_INDICATION      (0xA0)


/******************************************************************************/
/****** EVBME Enumerations                                               ******/
/******************************************************************************/
/* Status */
#define EVBME_SUCCESS          (0x00)
#define EVBME_FAIL             (0x01)
#define EVBME_UNKNOWN          (0x02)
#define EVBME_INVALID          (0x03)
#define EVBME_NO_ACCESS        (0x04)


/******************************************************************************/
/****** EVBME Attribute IDs                                              ******/
/******************************************************************************/
#define EVBME_RESETRF                      (0x00)
#define EVBME_CFGPINS                      (0x01)


/******************************************************************************/
/****** Global Parameters that can by set via EVBME_SET_request          ******/
/******************************************************************************/
extern uint8_t EVBME_SetVal[8];


/******************************************************************************/
/****** EVBME API Functions                                              ******/
/******************************************************************************/
int EVBMEInitialise(char *version, void *pDeviceRef);
int EVBME_SET_request(uint8_t Attribute, uint8_t AttributeLength, uint8_t *AttributeValue );


#endif // CASCODA_EVBME_H
