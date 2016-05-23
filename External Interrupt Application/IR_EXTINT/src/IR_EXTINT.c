/*
===============================================================================
 Name        : IR_EXTINT.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "timer.h"
#include "uart.h"
#include "extint.h"
#include "type.h"
#include <math.h>
#include <time.h>

#endif

#include <cr_section_macros.h>

#include <stdio.h>


extern uint32_t timer0_m0_counter, timer1_m0_counter;
extern uint32_t timer0_m1_counter, timer1_m1_counter;

int main(void)
{
	// Both PO.21 & PO.22 Are Active Low outputs
	LPC_GPIO0->FIODIR &= ~(1<<11); //set pin 2.11 as input
	LPC_GPIO0->FIODIR |= (1 << 21);
	LPC_GPIO0->FIOSET |= (1<<21);
	LPC_GPIO0->FIODIR |= (1 << 22);
	LPC_GPIO0->FIOCLR |= (1<<22);



	srand(time(NULL));
	int random;
	int i;
	int i2;
	int storage[4];//Time Waited for Interrupt
	int average;//average
	int var[4];//variance
	int std_dev;//standard deviation


	LPC_GPIO0->FIOSET |= (1<<22);
	printf("Test is about to begin!\n");
	LPC_GPIO0->FIOCLR |= (1<<22);
	printf("Press a button when Green LED activates!\n");

	i = 0;
	printf("Prepare yourself\n");
	delayMs(0,10000);
	LPC_GPIO0->FIOSET |= (1<<21);//Turn off LED1

	while(i < 6)
	{
		printf("Ready, Set,\n");
		random = rand() % 10000;//random number
		delayMs(0,random); // Incorporate delay of random number in ms
		LPC_TIM1->TCR = 0x02;// Obtained from Extint.h, initializes timer and interrupts
		LPC_TIM1->PR  = 0x00;
		LPC_TIM1->IR  = 0xff;
		LPC_TIM1->TCR = 0x01;
		LPC_GPIO0->FIOSET |= (1<<22);//turn off LED2  CLR
		printf("Go!!!!\n");
		while(LPC_TIM1->TCR == 0x01)
		{
			EINTInit();   //wait for external interrupt
			storage[i] = LPC_TIM1->TC;  // Store the elapsed time into storage array
		}
		LPC_GPIO0->FIOCLR |= (1<<22);//turn on LED2
		storage[i] = storage[i]/(9000000 / 1000-1);//Extracted from Timer.h, returns time elapsed in ms
		printf("Time: %i ms\n",storage[i]);
		i++;
	}

	LPC_GPIO0->FIOSET |= (1<<21);//turn off LED1
	average = (storage[0]+storage[1]+storage[2]+storage[3])/4;//calculate average

	for(i2 = 0;i2<4;i2++)//calculate variance for each value
	{
		var[i2] = storage[i2] - average;
		var[i2] = var[i2]*var[i2];
	}

	std_dev = sqrt((var[0]+var[1]+var[2]+var[3])/4);//calculate standard deviation
	printf("Average time: %i ms\n",average);
	printf("Standard deviation: %i ms\n",std_dev);
	printf("Test is Over\n");
	LPC_GPIO0->FIOCLR |= (1<<21);

	return 0;
}
