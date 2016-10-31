/*
 * @file cascoda_evbme.c
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

#include <stdint.h>

/******************************************************************************/
/****** Global Parameters that can by set via EVBME_SET_request          ******/
/******************************************************************************/
uint8_t EVBME_SetVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/******************************************************************************/
/******************************************************************************/
/****** EVBMEInitialise()                                                ******/
/******************************************************************************/
/****** Brief:  Initialises EVBME after Reset                            ******/
/******************************************************************************/
/****** Param:  version - Version String                                 ******/
/******************************************************************************/
/****** Return: SUCCESS/FAIL                                             ******/
/******************************************************************************/
/******************************************************************************/
int EVBMEInitialise(char *version, void *pDeviceRef)
{
	int status = 0;

	EVBME_SetVal[0] = 1;             // initialise/reset PIBs on higher layers

	return status;
} // End of EVBMEInitialise()
