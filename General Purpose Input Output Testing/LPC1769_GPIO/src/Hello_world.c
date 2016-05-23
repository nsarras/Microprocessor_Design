/*
===============================================================================
 Name        : Hello_world.c
 Author      : Jackson Zhao
 Version     : 1
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>

//Initialize the port and pin as outputs.
void GPIOinitOut(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIODIR |= (1 << pinNum);
	}
	else if (portNum == 1)
	{
		LPC_GPIO1->FIODIR |= (1 << pinNum);
	}
	else if (portNum == 2)
	{
		LPC_GPIO2->FIODIR |= (1 << pinNum);
	}
	else
	{
		puts("Not a valid port!\n");
	}
}

void setGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIOSET = (1 << pinNum);
		printf("Pin 0.%d has been set.\n",pinNum);
	}
	//Can be used to set pins on other ports for future modification
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

//Deactivate the pin
void clearGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIOCLR = (1 << pinNum);
		printf("Pin 0.%d has been cleared.\n", pinNum);
	}
	//Can be used to clear pins on other ports for future modification
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

int main(void)
{
    int check = 0;
    GPIOinitIn(0,2);
    GPIOinitOut(0,3);
    while(1){

    	check = readGPIO(0,2);
    	if(check == 1){
    		setGPIO(0,3);
    		printf("GPIO input %d",check);
    		printf("The LED is off \n");
    	}


    }
}



