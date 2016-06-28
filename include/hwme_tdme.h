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


#ifndef HWME_TDME_H
#define HWME_TDME_H

#define MAX_HWME_ATTRIBUTE_SIZE  16
#define MAX_TDME_ATTRIBUTE_SIZE  2


/******************************************************************************/
/****** HWME Enumerations                                                ******/
/******************************************************************************/
/* Status */
#define HWME_SUCCESS           (0x00)
#define HWME_UNKNOWN           (0x01)
#define HWME_INVALID           (0x02)
#define HWME_NO_ACCESS         (0x03)
#define HWME_MIN_STATUS         HWME_SUCCESS
#define HWME_MAX_STATUS         HWME_NO_ACCESS
/* WakeUpCondition */
#define HWME_WAKEUP_POWERUP    (0x00)
#define HWME_WAKEUP_WDT        (0x01)
#define HWME_WAKEUP_POFF_SLT   (0x02)
#define HWME_WAKEUP_POFF_GPIO  (0x03)
#define HWME_WAKEUP_STBY_SLT   (0x04)
#define HWME_WAKEUP_STBY_GPIO  (0x05)
#define HWME_WAKEUP_ACTIVE_SLT (0x06)
#define HWME_MIN_WAKEUP         HWME_WAKEUP_POWERUP
#define HWME_MAX_WAKEUP         HWME_WAKEUP_ACTIVE_SLT
/* LqiMode */
#define HWME_LQIMODE_CS        (0x00)
#define HWME_LQIMODE_ED        (0x01)


/******************************************************************************/
/****** HWME Attribute IDs                                               ******/
/******************************************************************************/
#define HWME_POWERCON          (0x00)
#define HWME_CHIPID            (0x01)
#define HWME_TXPOWER           (0x02)
#define HWME_CCAMODE           (0x03)
#define HWME_EDTHRESHOLD       (0x04)
#define HWME_CSTHRESHOLD       (0x05)
#define HWME_EDVALUE           (0x06)
#define HWME_CSVALUE           (0x07)
#define HWME_EDVALLP           (0x08)
#define HWME_CSVALLP           (0x09)
#define HWME_FREQOFFS          (0x0A)
#define HWME_MACTIMER          (0x0B)
#define HWME_RANDOMNUM         (0x0C)
#define HWME_TEMPERATURE       (0x0D)
#define HWME_HSKEY             (0x0E)
#define HWME_SYSCLKOUT         (0x0F)
#define HWME_LQIMODE           (0x10)
#define HWME_MIN_ATTRIBUTE     HWME_POWERCON
#define HWME_MAX_ATTRIBUTE     HWME_LQIMODE


/******************************************************************************/
/****** TDME Enumerations                                                ******/
/******************************************************************************/
/* Hardware Error Messages from TDME_ERROR_indication */
#define TDME_ERR_NO_ERROR     (0x00)  // Everything OK
#define TDME_ERR_LO_UNLOCK    (0x10)  // LO Unlocked
#define TDME_ERR_SPI_INVALID  (0x20)  // Invalid SPI Message
#define TDME_ERR_EX_STOF      (0x30)  // Exception: Stack Overflow
#define TDME_ERR_EX_STUF      (0x40)  // Exception: Stack Underflow
#define TDME_ERR_EX_DIVBY0    (0x50)  // Exception: Divide by 0
#define TDME_ERR_DMA_ACCESS   (0x60)  // DMA Access Overflow/Underflow (+0x0F)
#define TDME_ERR_UNEXP_IRQ    (0x70)  // Unexpected Interrupt (+0x0F)
#define TDME_ERR_MALLOC_FAIL  (0x80)  // Memory Allocation Failure
#define TDME_ERR_SYSTEM_ERROR (0x90)  // System Error
/* Status */
#define TDME_SUCCESS          (0x00)
#define TDME_UNKNOWN          (0x01)
#define TDME_INVALID          (0x02)
#define TDME_NO_ACCESS        (0x03)
#define TDME_LO_ERROR         (0x04)
#define TDME_FCS_ERROR        (0x05)
#define TDME_SHR_ERROR        (0x06)
#define TDME_PHR_ERROR        (0x07)
#define TDME_MIN_STATUS       TDME_SUCCESS
#define TDME_MAX_STATUS       TDME_PHR_ERROR
/* Test Modes */
#define TDME_TEST_OFF         (0x00)
#define TDME_TEST_IDLE        (0x01)
#define TDME_TEST_TX          (0x02)
#define TDME_TEST_RX          (0x03)
#define TDME_TEST_ED          (0x04)
#define TDME_TEST_LO_1        (0x05)
#define TDME_TEST_LO_2        (0x06)
#define TDME_TEST_LO_3        (0x07)
#define TDME_MIN_TESTMODE     TDME_TEST_OFF
#define TDME_MAX_TESTMODE     TDME_TEST_LO_3
/* Tx Packet PHY Payload Data Type */
#define TDME_TXD_RANDOM       (0x00)
#define TDME_TXD_SEQRANDOM    (0x01)
#define TDME_TXD_COUNT        (0x02)
#define TDME_TXD_APPENDED     (0x03)
#define TDME_MIN_TXD          TDME_TXD_RANDOM
#define TDME_MAX_TXD          TDME_TXD_APPENDED


/******************************************************************************/
/****** TDME Attribute IDs                                               ******/
/******************************************************************************/
#define TDME_CHANNEL          (0x00)
#define TDME_TX_CONFIG        (0x01)
#define TDME_ED_CONFIG        (0x02)
#define TDME_RX_CONFIG        (0x03)
#define TDME_LO_1_CONFIG      (0x04)
#define TDME_LO_2_CONFIG      (0x05)
#define TDME_ATM_CONFIG       (0x06)
#define TDME_MIN_ATTRIBUTE    TDME_CHANNEL
#define TDME_MAX_ATTRIBUTE    TDME_ATM_CONFIG


/******************************************************************************/
/****** TDME Enumerations for Hardware Error Messages                    ******/
/******************************************************************************/
#define TDME_ERR_NO_ERROR     (0x00)  // Everything OK
#define TDME_ERR_LO_UNLOCK    (0x10)  // LO Unlocked
#define TDME_ERR_SPI_INVALID  (0x20)  // Invalid SPI Message
#define TDME_ERR_EX_STOF      (0x30)  // Exception: Stack Overflow
#define TDME_ERR_EX_STUF      (0x40)  // Exception: Stack Underflow
#define TDME_ERR_EX_DIVBY0    (0x50)  // Exception: Divide by 0
#define TDME_ERR_DMA_ACCESS   (0x60)  // DMA Access Overflow/Underflow (+0x0F)
#define TDME_ERR_UNEXP_IRQ    (0x70)  // Unexpected Interrupt (+0x0F)
#define TDME_ERR_MALLOC_FAIL  (0x80)  // Memory Allocation Failure
#define TDME_ERR_SYSTEM_ERROR (0x90)  // System Error


/******************************************************************************/
/****** HWME/TDME/MAC Additional Enumerations                            ******/
/******************************************************************************/
/* CCA Mode */
#define CCAM_EDORCS     (0)   /* level above ED or CS */
#define CCAM_ED         (1)   /* level above ED */
#define CCAM_CS         (2)   /* level above CS */
#define CCAM_EDANDCS    (3)   /* level above ED and CS */


#endif // HWME_TDME_H
