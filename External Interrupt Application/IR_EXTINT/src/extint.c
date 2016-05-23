/****************************************************************************
 *   $Id:: extint.c 5670 2010-11-19 01:33:16Z usb00423                      $
 *   Project: NXP LPC17xx EINT example
 *
 *   Description:
 *     This file contains EINT code example which include EINT 
 *     initialization, EINT interrupt handler, and APIs for EINT.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "extint.h"
#include "timer.h"
#include "uart.h"
#include <time.h>
volatile uint32_t eint0_counter;
extern uint32_t timer0_m0_counter, timer1_m0_counter;
extern uint32_t timer0_m1_counter, timer1_m1_counter;
int key_count1 = 0;
int key_count2 = 1;
int key_count = 0;

/*****************************************************************************
** Function name:		EINT0_Handler
**
** Descriptions:		external INT handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/


void EINT1_IRQHandler ()
{
	LPC_SC->EXTINT = EINT1;
	printf("Interrupt Triggered\n");
	LPC_TIM1->TCR = 0x00;//STOP TIMER
	/*printf("Interrupt Triggered\n");
	LPC_GPIO0->FIOCLR |= (1<<2);
	srand(time(NULL));
	int r = rand() % 10000;//random number between 0 and 10000 (milliseconds)
	delayMs(0,r);
	LPC_GPIO0->FIOSET |= (1<<2);
	clock_t t;
	t = clock();
	while((LPC_GPIO0->FIOPIN & (1 << 21))>>21 == 1)
		{
			//delayMs(0,1);
		}
	t = clock() - t;
	double diff = ((double)t)/ CLOCKS_PER_SEC;
	printf("Time Elapsed: %f seconds\n",diff);
	delayMs(0,1000);
	printf("Interrupt finished. Press remote to try again.\n");
	LPC_GPIO0->FIOCLR |= (1<<2);
	/*LPC_GPIO0->FIODIR |= (1<<3);
	if(LPC_GPIO2->FIOPIN &(1<<11))
	{

		key_count ++;  // key_count +1 when receive external interrupt
	    delayMs(0,500);  //delay used as debouncer
	}

	if( key_count == (key_count1 + key_count2))
	{
		if(LPC_GPIO0->FIOPIN & (1<<2))
		{
		   LPC_GPIO0->FIOCLR |= (1<<2);  //turn off led if it was on
		}
		else
		LPC_GPIO0->FIOSET |= (1<<2);   //turn on led if it was off
		key_count1 += key_count2;
		key_count2 ++;                 //increment the count and calculate the number of times needed to turn on and turn off led
		                               //press button 1 time to turn on LED,
		                               //             2 times to turn off LED,
		                               //             3 times to turn on LED
                                       //             4 times to turn off LED ...
	}*/

    LPC_GPIOINT->IO2IntEnR = ((0x01 <<11));
	LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
	LPC_GPIOINT->IO0IntClr = 0xFFFFFFFF;

}
/*****************************************************************************
** Function name:		EINTInit
**
** Descriptions:		Initialize external interrupt pin and
**						install interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the interrupt
**						handler can't be installed to the VIC table.
** 
*****************************************************************************/
uint32_t EINTInit( void )
{
	 LPC_PINCON->PINSEL4 &= ~(3 << 22 );  //set P2.11 as EINT1
	 LPC_PINCON->PINSEL4 |= (1 << 22 );
	 LPC_PINCON->PINMODE4 = 0;						// for making pull-up use 00
	 LPC_GPIOINT->IO2IntEnR |= ((0x01 <<11));				/* Port2.10 is rising edge. */
	 LPC_GPIOINT->IO2IntEnF &= ~((0x01 <<11));
	 LPC_SC->EXTMODE = EINT0_EDGE | EINT3_EDGE ;      /* INT1 edge trigger */
	 LPC_SC->EXTPOLAR |= 0;              /* INT0 is falling edge by default */
	 NVIC_EnableIRQ(EINT1_IRQn);
	 return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

