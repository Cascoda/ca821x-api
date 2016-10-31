/*
 * @file cascoda_evbme.h
 * @brief Cascoda EVBME function code.
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
