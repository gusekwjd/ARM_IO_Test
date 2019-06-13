#include "board.h"
#include "AT91SAM7x.h"
#include "myLIB.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <pio/pio.h>
#include <aic/aic.h>
#include <utility\trace.h>
#include <cs8900a/cs8900a.h>

//For Switch
#define LEFT	1
#define RIGHT	2

#define ROW1 PA8
#define ROW2 PA9
#define ROW3 PA13
#define ROW4 PA2
#define ROW5 PA14
#define ROW6 PA4
#define ROW7 PA5
#define ROW8 PA11

#define COL1 PA15
#define COL2 PA10
#define COL3 PA0
#define COL4 PA12
#define COL5 PA7
#define COL6 PA1
#define COL7 PA6
#define COL8 PA3


unsigned int 	Key_Count=0,Pre_Key_Data=0;
unsigned char Switch_Check(void);
unsigned char Port_Flag=0;
unsigned int	Count=0; 


int mode=0;
int trigger_request=0;
int ten_us_count=0; 
//============================================================================
//  Function  : PIT Interrupt
//============================================================================
void Isr_PIT(void)
{
    volatile unsigned int pit_pivr;
	if((rPIT_SR & 1) != 0)  //The Periodic Interval timer has reached PIV since the last read of PIT_PIVR
    {
		pit_pivr = rPIT_PIVR; //Reads Periodic Interval Timer Value Register - Clears PITS in PIT_SR
//		Count++;
//		if(Count==100)
//		{
//			Count=0;
			if(Port_Flag==0)
			{
				rPIO_SODR_B=(LED1|LED2|LED3);
				Port_Flag=1;
			}
			else
			{
				
				rPIO_CODR_B=(LED1|LED2|LED3);
				Port_Flag=0;
			}	
//		}		
	}
}
void 	PIT_Interrupt_Setup(void) 
{
	unsigned int	tmp=0;

    rAIC_IECR = (1<<1);

	// System Advanced Interrupt Controller
    rAIC_SMR1 = (1<<5) +  (7<<0);  //Edge Trigger, Prior 7
    rAIC_SVR1 = (unsigned)Isr_PIT;

	// System Periodic Interval Timer (PIT) Mode Register (MR)

	// PITEN(24) - Periodic Interval Timer Enabled
	// unsigned int PITEN = (1<<24);

	// PITIEN(25) - Periodic Interval Timer Interrupt Enabled
	// unsigned int PITIEN = (1<<25);

	// PIV(19:0) - Periodic Interval Time
	// will be compared with 20-bit CPIV (Counter of Periodic Interval Timer)
    tmp=(48000000/16/100)&0xFFFFF;         // T=30Hz
	// unsigned int PIV = 0;
	// PIV = (48000000/16/100)&0xFFFFF;

    rPIT_MR=(1<<25)+(1<<24)+(tmp<<0);      // Enable PIT, Disable Interrupt
	// rPIT_MR = PITIEN + PITEN + PIV;
}


void Port_Setup(void)
{
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );
	
	
	//DoteMetrix
	
	
	// Switch (Port A: 8,9)
	AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PORTA); // output mode
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOA,PORTA  ); // pull-up
	
}       
void HW_delay_ten_us(unsigned int ten_us)
{
	// special case
	if(ten_us == 0) return;
	// start time
	ten_us_count = 0;
	// enable PIT interrupt
	AT91F_PITEnableInt(AT91C_BASE_PITC);
	// wait for ms
	while(ten_us_count < ten_us);
	// disable PIT interrupt
	AT91F_PITDisableInt(AT91C_BASE_PITC);
}          

int main()
{
 	//Port set up
 	Port_Setup();
   	rPIO_CODR_A=PORTA; 

	  while(1) 
	  {
	     rPIO_CODR_A=PORTA;   
	  	
	  	  rPIO_SODR_A=ROW1;
	 	 rPIO_SODR_A=COL1|COL4|COL5|COL8;		
	   			
		 Delay(10);
		 
		 
		 rPIO_CODR_A=PORTA;	  
	  	
	 	 rPIO_SODR_A=COL2|COL3|COL6|COL7;		
	     
	     rPIO_SODR_A=ROW2; 	     	
		
		 Delay(10);
		 
		  rPIO_CODR_A=PORTA;	  
	  	
	 	 rPIO_SODR_A=COL2|COL3|COL4|COL5|COL6|COL7;		
	     
	     rPIO_SODR_A=ROW3|ROW4|ROW5; 	     	
		
		 Delay(10);
		 
		 
		  rPIO_CODR_A=PORTA;	  
	  	
	 	 rPIO_SODR_A=COL1|COL3|COL4|COL5|COL6|COL8;		
	     
	     rPIO_SODR_A=ROW6; 	     	
		
		 Delay(10);
		 
		 
		 rPIO_CODR_A=PORTA;	  
	  	
	 	 rPIO_SODR_A=COL1|COL2|COL4|COL5|COL7|COL8;		
	     
	     rPIO_SODR_A=ROW7; 	     	
		
		Delay(10);
		 
		 
		  rPIO_CODR_A=PORTA;	  
	  	
	 	 rPIO_SODR_A=COL1|COL2|COL3|COL6|COL7|COL8;		
	     
	     rPIO_SODR_A=ROW8; 	     	
		
		Delay(10);
	 
	 
		 
	  } 
	  
}

