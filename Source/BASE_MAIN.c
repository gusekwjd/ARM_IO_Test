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

unsigned int 	Key_Count=0,Pre_Key_Data=0;
unsigned char Switch_Check(void);
unsigned char Port_Flag=0;
unsigned int	Count=0; 

//Stop watch global var
int s=0;
int m=0;
int h=0;
int ms=0;

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
	// PMC (Power Management Clock) enables peripheral clocks
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOB );
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );
	
	// Enable PIO in output mode: Port A 0-7
	//AT91F_PIO_CfgOutput( AT91C_BASE_PIOA,  PORTA);

	// LED (Port B: 28-30)
	AT91F_PIO_CfgOutput( AT91C_BASE_PIOB, LED1|LED2|LED3 ); // output mode
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOB, LED1|LED2|LED3 ); // pull-up

	// Switch (Port A: 8,9)
	AT91F_PIO_CfgInput( AT91C_BASE_PIOA, SW1|SW2 ); // output mode
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, SW1|SW2 ); // pull-up

	//Ultra 
	AT91F_PIO_CfgInput( AT91C_BASE_PIOA, PA0 ); // output mode
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, PA1 ); // pull-up

	
	//AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,1<<AT91C_ID_PIOA);

	//AT91F_PIO_SetOutput(AT91C_BASE_PIOA, (1<<13));
	//AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, (1<<13));
	
}
/*
void Read_For_Setup_CMOS(void)
{

	//Read address Reset
	rPIO_CODR_A=FIFO_RD_RST;			
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
	rPIO_SODR_A=FIFO_RD_RST;

	//Write On		
	//rPIO_SODR_A=XCLK_ON;
	rPIO_SODR_A=HREF_SYNC;
			
	//Until Wait Low
	while((rPIO_PDSR_A & 0x00000004)){}
	//Until Wait High
	while(!(rPIO_PDSR_A & 0x00000004)){}
			
	//Write Off		
	//rPIO_CODR_A=HREF_SYNC;
			
	//CS_LOW
	rPIO_CODR_A=FIFO_CS;

}

void CMOS_Read_Clk(void)
{
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
}
*/

unsigned char Switch_Check(void)
{
unsigned char Result=0;

	if(!(rPIO_PDSR_A & SW1)) Result=LEFT;
	else if(!(rPIO_PDSR_A & SW2)) Result=RIGHT;
	
	
	if(Pre_Key_Data==Result) Key_Count++;
	else Key_Count=0;
	
	Pre_Key_Data=Result;
	return	Result;
}
//-----------------------------------------------------------------------------
/// Main Procedure
//-----------------------------------------------------------------------------
// PIT interrupt service routine
volatile unsigned int ten_us_count = 0;
void PIT_ISR()
{
	// Clear PITS
	AT91F_PITGetPIVR(AT91C_BASE_PITC);

	// increase ms_count
	ten_us_count++;
}

// Initialize PIT and interrupt
void PIT_initiailize()
{
	// enable peripheral clock for PIT
	AT91F_PITC_CfgPMC();

	// set the period to be every 1 msec in 48MHz
	AT91F_PITInit(AT91C_BASE_PITC, 1, 48);

	// PIV (Periodic Interval Value) = 3000 clocks = 1 msec
	// MCK/16 = 48,000,000 / 16 = 3,000,000 clocks/sec
	//30 clocks / 10 usec
	AT91F_PITSetPIV(AT91C_BASE_PITC, 30-1);

	// disable PIT periodic interrupt for now
	AT91F_PITDisableInt(AT91C_BASE_PITC);

	// interrupt handler initializatioin
	AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, AT91C_ID_SYS, 7, 1, PIT_ISR);

	// enable the PIT interrupt
	AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_SYS);
}

// delay in ms by PIT interrupt


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

//PIO interrupt service routine
void PIO_ISR()
{
	//Reset the stop watch
	s=0;
	m=0;
	h=0;
	ms=0;
	
}

void interrupt_setup()
{
	//SW1을 input
	AT91F_PIO_InputFilterEnable(AT91C_BASE_PIOA,SW1);
	
	//Set interrupt SW1
	AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,SW1);
	
	//Set callback function
	//PIOA 에 인터럽트가 걸리면 
	AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,AT91C_ID_PIOA,7,1,PIO_ISR);
	
	//Enable AIC
	//AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
}


void TC_initialize()
{
  // Enable peripheral clock in PMC for Timer Counter 0
  AT91F_TC0_CfgPMC();

  // Configure Timer Counter 0 with Software Trigger Effect on TIOA
// MCK/2 = 48,000,000 / 2 = 24,000,000 clocks/sec
  TC_Configure(AT91C_BASE_TC0, AT91C_TC_CLKS_TIMER_DIV3_CLOCK |
                               AT91C_TC_ASWTRG); 
}
                   

int main()
{
  int n =0;

  // Port set up
  Port_Setup();
   
  // UART 
  DBG_Init();
  Uart_Printf("Ultrasound - Test\n");

  // PIT setup
  PIT_initiailize();
  // Timer counter
  TC_initialize();

  while(1) 
  {
    //Uart_Printf("iter = %d  ", n);
    //rPIO_SODR_B=(LED1|LED2|LED3);
    

    rPIO_CODR_A=(PA0|PA1); 
    
    //[falling 방식]
    //1-트리거핀 on [PA0]
    rPIO_SODR_A=(PA0);
    //2-10us dellay
	HW_delay_ten_us(1);
    //3-트리거핀 off
    rPIO_CODR_A=(PA0);
    //4-에코핀 기다리기(on인지)[PA1] 
    while(1)
    {
     	//5-에코핀이 on이면 타이머를 키기 
     	if(AT91F_PIO_IsInputSet(AT91C_BASE_PIOA,PA1))
     	{
     		break;
     	} 		
    }
    
    TC_Start(AT91C_BASE_TC0);
    
    while(1)
    {
      	//6-에코핀이 off면 타이머를 끄기 
		if(!AT91F_PIO_IsInputSet(AT91C_BASE_PIOA,PA1))
		{
			break;     	
     	}
     }
     
     TC_Stop(AT91C_BASE_TC0);
    //8-시간을 재서 거리재서 출력하기 
	Uart_Printf("TC_CV = %lf clocks\n",(double)(AT91C_BASE_TC0->TC_CV)/(58*1.5));

	/*
    // 타이머시작
    TC_Start(AT91C_BASE_TC0);
    
	// 딜레이
    HW_delay_ten_us(100);

    // 타이머스탑
    TC_Stop(AT91C_BASE_TC0);
    
    //오버플로우 
     if (AT91C_BASE_TC0->TC_SR& AT91C_TC_COVFS)
    {
      Uart_Printf("Overflow - ");
    }
    else
    {
      Uart_Printf("Normal - ");
    }
    
    // 타이머값
    // (data sheet)
    // AT91C_TC_CLKS_TIMER_DIV1_CLOCK = MCK/2 = 48MHz / 2 = 24MHz
    // TC_CV/24,000 = 1 msec
    
    // AT91C_TC_CLKS_TIMER_DIV1_CLOCK = MCK/1000 = 48kHz, TC_CV/48,000 = 1 sec
    // AT91C_TC_CLKS_TIMER_DIV2_CLOCK = MCK/? = 48kHz, TC_CV/48,000 = 1 sec
    
    
    Uart_Printf("TC_CV = %u clocks\n", AT91C_BASE_TC0->TC_CV);
    
    
    //rPIO_CODR_B=(LED1|LED2|LED3);
    HW_delay_ten_us(50000);
    n++;
    */
  } 
}

