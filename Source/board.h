
/* ============================================================================

  [ Module Name ]
  BOARD.H

  [ Description ]


  Copyright (c) 2005 FirePooh, Inc. All Rights Reserved.

============================================================================ */

/* ============================================================================

  [ History ]

  when       who          what, where, why 
  ---------- ------------ ---------------------------------------------------
  2005/10/21 FIREPOOH     Initial release

============================================================================ */


/*
*********************************************************************************************************
*                                   INCLUDES
*********************************************************************************************************
*/

#ifndef Board_h
#define Board_h

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "AT91SAM7X512.h"
#include "lib_AT91SAM7X512.h"
#include "datatype.h"
#include "BASE_MAIN.h"

//#include "BASE_EEPROM.h"
//#include "BASE_LCD.h"
//#include "BASE_GLCD.h"
//#include "BASE_MEMORY.h"
//#include "BASE_SFLASH.h"
//#include "BASE_DBG.h"
//#include "BASE_TIMER.h"
//#include "BASE_ADC.h"
//#include "BASE_BUZZ.h"
//#include "BASE_UART.h"
//#include "BASE_IO.h"
//#include "BASE_USB.h"
//#include "BASE_ADS7846.h"
//#include "BASE_TEST.h"

#define true    (-1)
#define false   (0)
#define BOARD_NAME "AT91SAM7X-EK"

/*-------------------------------*/
/* SAM7Board Memories Definition */
/*-------------------------------*/
// The AT91SAM7X256 embeds a 16-Kbyte SRAM bank, and 64 K-Byte Flash

#define  INT_SARM           0x00200000
#define  INT_SARM_REMAP     0x00000000

#define  INT_FLASH          0x00000000
#define  INT_FLASH_REMAP    0x01000000

#define  FLASH_PAGE_NB    512
#define  FLASH_PAGE_SIZE  128

//For Serial Camera Control Bus
#define CHANGE_REG_NUM 167 
/*-----------------*/
/* For CS8900A */
/*-----------------*/
#define D0					(1<<0)  //PORTB
#define D1					(1<<1)	//PORTB				
#define D2					(1<<2) //PORTB
#define D3					(1<<3) //PORTB
#define D4					(1<<4) //PORTB
#define D5					(1<<5) //PORTB
#define D6					(1<<6) //PORTB
#define D7					(1<<7) //PORTB

#define D8					(1<<8) //PORTB
#define D9					(1<<9)	//PORTB 		 		
#define D10					(1<<10) //PORTB
#define D11					(1<<11) //PORTB
#define D12					(1<<12) //PORTB
#define D13					(1<<13) //PORTB
#define D14					(1<<14) //PORTB
#define D15					(1<<15) //PORTB

#define	Data_Bus			(D0|D1|D2|D3|D4|D5|D6|D7|D8|D9|D10|D11|D12|D13|D14|D15)
#define DBUS_L				(D0|D1|D2|D3|D4|D5|D6|D7)
#define DBUS_H				(D8|D9|D10|D11|D12|D13|D14|D15)

//Port B

//Out
#define A1					(1<<25) //PORTB
#define A2					(1<<26)	//PORTB				
#define A3					(1<<27) //PORTB

#define	Address_Bus			(A1|A2|A3)
//Port B

#define	CS					(1<<18) //PORTB
#define	nWE					(1<<17)	//PORTB
#define	nRD					(1<<16) //PORTB
#define	Control				(CS|nWE|nRD)

//Input
#define	IRQ_LAN				(1<<20)	//PORTB
#define	LAN_nWait			(1<<19)	//PORTB


//PORTA IO
#define PA0					(1<<0)
#define PA1					(1<<1)
#define PA2					(1<<2)
#define PA3					(1<<3)
#define PA4					(1<<4)
#define PA5					(1<<5)
#define PA6					(1<<6)
#define PA7					(1<<7)
#define PA8					(1<<8)
#define PA9					(1<<9)
#define PA10					(1<<10)
#define PA11					(1<<11)
#define PA12					(1<<12)
#define PA13					(1<<13)
#define PA14					(1<<14)
#define PA15					(1<<15)

#define SW1					(1<<8)
#define SW2					(1<<9)


#define PORTA				(PA0|PA1|PA2|PA3|PA4|PA5|PA6|PA7|PA8|PA9|PA10|PA11|PA12|PA13|PA14|PA15)
#define SWITCH				(SW1|SW2)



//PORTB
#define LED1				(1<<28)
#define LED2				(1<<29)
#define LED3				(1<<30)

/*------------------*/
/* USART Definition */
/*------------------*/
/* SUB-D 9 points J3 DBGU*/
#define DBGU_RXD            AT91C_PA9_DRXD
#define DBGU_TXD            AT91C_PA10_DTXD
#define AT91C_DBGU_BAUD     115200   // Baud rate

#define US_RXD_PIN    AT91C_PA5_RXD0    /* JP9 must be close */
#define US_TXD_PIN    AT91C_PA6_TXD0    /* JP7 must be close */
#define US_RTS_PIN    AT91C_PA7_RTS0    /* JP8 must be close */
#define US_CTS_PIN    AT91C_PA8_CTS0    /* JP6 must be close */

/*--------------*/
/* Master Clock */
/*--------------*/

#define EXT_OC          18432000   // Exetrnal ocilator MAINCK
#define MCK             47923200   // MCK (PLLRC div by 2)
//#define MCK				95846400//48000000
#define MCKKHz          (MCK/1000) //
//#define BOARD_MCK              95846400
#define BOARD_MCK              47923200

#define BOARD_MAINOSC           18432000

/*------------------*/
/* CAN Definition   */
/*------------------*/
#define AT91B_CAN_TRANSCEIVER_RS  (1<<2)    // PA2
/// CAN Definition
/// RS: Select input for high speed mode or silent mode
#define PIN_CAN_TRANSCEIVER_RS  {1<<2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// TXD: Transmit data input
#define PINS_CAN_TRANSCEIVER_TXD  {1<<20, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// RXD: Receive data output
#define PINS_CAN_TRANSCEIVER_RXD  {1<<19, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
//#define PIN_USART0_RXD  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 TXD pin definition.
//#define PIN_USART0_TXD  {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SCK0 pin definition
//#define PIN_USART0_SCK  {1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO   {1 << 16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI   {1 << 17, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK   {1 << 18, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
//#define PINS_SPI0       PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0  {1 << 12, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 chip select 1 pin definition.
//#define PIN_SPI0_NPCS1  {1 << 13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// PWMC PWM0 pin definition.
#define PIN_PWMC_PWM0  {1 << 19, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM1 pin definition.
#define PIN_PWMC_PWM1  {1 << 20, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM2 pin definition.
#define PIN_PWMC_PWM2  {1 << 21, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM3 pin definition.
#define PIN_PWMC_PWM3  {1 << 22, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/// TWI pins definition.
#define PINS_TWI  {0x00000C00, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

#define PINS_DBGU  {0x18000000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// Board EMAC Power Down control pin
//#define BOARD_EMAC_PIN_PWRDN {(1<<18), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// Board EMAC mode - RMII/MII ( 1/0 )
//#define BOARD_EMAC_MODE_RMII 0 // MII is the default mode
/// The PIN list of PIO for EMAC
//#define BOARD_EMAC_PINS     {0x3FFFF, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// The power up reset latch PIO for PHY
//#define BOARD_EMAC_PIN_TEST   {(1<<15), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
//#define BOARD_EMAC_PIN_RMII   {(1<<16), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
// We force the address
//(1<<5) PHY address 0, (1<<6) PHY address 1, (1<<13) PHY address 2,
//(1<<14) PHY address 3, (1<<4) PHY address 4
//#define BOARD_EMAC_PINS_PHYAD {(1<<6)|(1<<13)|(1<<14)|(1<<4),\
//                               AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT},\
//                              {(1<<5), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}
//#define BOARD_EMAC_PIN_10BT   {(1<<17), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
//#define BOARD_EMAC_PIN_RPTR   {(1<< 7), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/// The PIN Configure list for EMAC on power up reset (MII)
//#define BOARD_EMAC_RST_PINS BOARD_EMAC_PINS_PHYAD, \
//                            BOARD_EMAC_PIN_TEST, BOARD_EMAC_PIN_RMII, \
 //                           BOARD_EMAC_PIN_10BT, BOARD_EMAC_PIN_RPTR

/// The runtime pin configure list for EMAC
//#define BOARD_EMAC_RUN_PINS BOARD_EMAC_PINS
//------------------------------------------------------------------------------

#endif /* Board_h */
