/*
===============================================================================
 Name        : SSP_Test.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "ssp.h"
#include <stdio.h>

/* Be careful with the port number and location number, because
some of the location may not exist in that port. */
#define PORT_NUM			1
#define LOCATION_NUM		0
#define USE_CS 				1
#define USE_NCS				0
#define OP_ID              0x9F
uint8_t src_addr[SSP_BUFSIZE];
uint8_t dest_addr[SSP_BUFSIZE];

/*****************************************************************************
 ** Function name:		LoopbackTest
 **
 ** Descriptions:		Loopback test
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void LoopbackTest( uint32_t portnum, uint32_t location )
{
	uint32_t i;

#if !USE_CS
	/* Set SSEL pin to output low. */
	SSP_SSELToggle( portnum, 0 );
#endif
	i = 0;
	while ( i <= SSP_BUFSIZE )
	{
		/* to check the RXIM and TXIM interrupt, I send a block data at one time
	based on the FIFOSIZE(8). */
		SSPSend( portnum, (uint8_t *)&src_addr[i], FIFOSIZE );
		/* If RX interrupt is enabled, below receive routine can be
	also handled inside the ISR. */
		SSPReceive( portnum, (uint8_t *)&dest_addr[i], FIFOSIZE );
		i += FIFOSIZE;
	}
#if !USE_CS
	/* Set SSEL pin to output high. */
	SSP_SSELToggle( portnum, 1 );
#endif

	/* verifying write and read data buffer. */
	for ( i = 0; i < SSP_BUFSIZE; i++ )
	{
		if ( src_addr[i] != dest_addr[i] )
		{
			while( 1 );			/* Verification failed */
		}
	}
	return;
}

/*****************************************************************************
 ** Function name:		SEEPROMTest
 **
 ** Descriptions:		Serial EEPROM(Atmel 25xxx) test
 **
 ** parameters:			None
 ** Returned value:		None
 **
 *****************************************************************************/
void SEEPROMTest( uint32_t portnum, uint32_t location )
{
	uint32_t i, timeout;
#if SSP_DEBUG
	uint8_t temp[2];
#endif

	if ( portnum == 1 )
	{
		LPC_GPIO0->FIODIR |= (0x1<<16);		/* SSP1, P0.16 defined as Outputs */
	}
	else
	{
		LPC_GPIO0->FIODIR |= (0x1<<6);		/* SSP0 P0.6 defined as Outputs */
	}
	SSP_SSELToggle( portnum, 0 );

	/* Test Atmel 25016 SPI SEEPROM. */
	src_addr[0] = WREN;			/* set write enable latch */
	SSPSend( portnum, (uint8_t *)src_addr, 1 );
	SSP_SSELToggle( portnum, 1 );

	for ( i = 0; i < DELAY_COUNT; i++ );	/* delay minimum 250ns */

	SSP_SSELToggle( portnum, 0 );
	src_addr[0] = RDSR;	/* check status to see if write enabled is latched */
	SSPSend( portnum, (uint8_t *)src_addr, 1 );
	SSPReceive( portnum, (uint8_t *)dest_addr, 1 );
	SSP_SSELToggle( portnum, 1 );
	if ( dest_addr[0] & (RDSR_WEN|RDSR_RDY) != RDSR_WEN )
		/* bit 0 to 0 is ready, bit 1 to 1 is write enable */
	{
		while ( 1 );
	}

	for ( i = 0; i < SSP_BUFSIZE; i++ )	/* Init RD and WR buffer */
	{
		src_addr[i+3] = i;	/* leave three bytes for cmd and offset(16 bits) */
		dest_addr[i] = 0;
	}

	/* please note the first two bytes of WR and RD buffer is used for
  commands and offset, so only 2 through SSP_BUFSIZE is used for data read,
  write, and comparison. */
	SSP_SSELToggle( portnum, 0 );
	src_addr[0] = WRITE;	/* Write command is 0x02, low 256 bytes only */
	src_addr[1] = 0x00;	/* write address offset MSB is 0x00 */
	src_addr[2] = 0x00;	/* write address offset LSB is 0x00 */
	SSPSend( portnum, (uint8_t *)src_addr, SSP_BUFSIZE );
	SSP_SSELToggle( portnum, 1 );

	for ( i = 0; i < 0x30000; i++ );	/* delay, minimum 3ms */

	timeout = 0;
	while ( timeout < MAX_TIMEOUT )
	{
		SSP_SSELToggle( portnum, 0 );
		src_addr[0] = RDSR;	/* check status to see if write cycle is done or not */
		SSPSend( portnum, (uint8_t *)src_addr, 1);
		SSPReceive( portnum, (uint8_t *)dest_addr, 1 );
		SSP_SSELToggle( portnum, 1 );
		if ( (dest_addr[0] & RDSR_RDY) == 0x00 )	/* bit 0 to 0 is ready */
		{
			break;
		}
		timeout++;
	}
	if ( timeout == MAX_TIMEOUT )
	{
		while ( 1 );
	}

	for ( i = 0; i < DELAY_COUNT; i++ );	/* delay, minimum 250ns */

	SSP_SSELToggle( portnum, 0 );
	src_addr[0] = READ;		/* Read command is 0x03, low 256 bytes only */
	src_addr[1] = 0x00;		/* Read address offset MSB is 0x00 */
	src_addr[2] = 0x00;		/* Read address offset LSB is 0x00 */
	SSPSend( portnum, (uint8_t *)src_addr, 3 );
	SSPReceive( portnum, (uint8_t *)&dest_addr[3], SSP_BUFSIZE-3 );
	SSP_SSELToggle( portnum, 1 );

	/* verifying, ignore the difference in the first two bytes */
	for ( i = 3; i < SSP_BUFSIZE; i++ )
	{
		if ( src_addr[i] != dest_addr[i] )
		{
			while( 1 );			/* Verification failed */
		}
	}
	return;
}


void initSSP1(void){
	//power up spi1
	LPC_SC->PCONP |= (1<<10);
	//01 PCLK_peripheral = CCLK.. 01, since it's CCLK/1
	LPC_SC->PCLKSEL0 &= ~(3<<20);
	LPC_SC->PCLKSEL0 |= (1<<20);

	//P0.6 is used as a GPIO output and acts as a Slave select
	LPC_GPIO0->FIODIR |= (1<<6);
	LPC_GPIO0->FIOSET = (1<<6);
	//P0.7:9 init
	LPC_PINCON->PINSEL0 &= ~((3<<18)|(3<<16)|(3<<14));
	LPC_PINCON->PINSEL0 |= ((2<<18)|(2<<16)|(2<<14));
	//data size set to 8 bits
	LPC_SSP1->CR0 = 0x07;
	//For AT45 flash SI pin is always latched on the rising edge of SCK, while output data
	//on the SO pin is always clocked out on the falling edge of SCK.
	//MS=0 (Master), SSE =1
	LPC_SSP1->CR1 = 0x2;
	LPC_SSP1->CPSR = 8; //SCK Frequency for Continuous Array Read(Low Frequency) is 33Mhx max. here we are setting it below it.
}

uint8_t SSP1exchangeByte(uint8_t out){
	LPC_SSP1->DR = out;
	while(LPC_SSP1->SR & (1<<4));
	return LPC_SSP1->DR;
}
/******************************************************************************
 **   Main Function
 ******************************************************************************/
#define SPI_DELAY 1000
int main (void)
{
	int i;
	initSSP1();
	//enable_timer(0);
	for ( i = 0; i < 10000; i++ );
	printf("Device ID\n");
	LPC_GPIO0->FIOCLR = (1<<6);     // 0.6 port 0 pin 6
	printf("\n 0\t %x",SSP1exchangeByte(0x9f));
	printf("\n 1\t %x",SSP1exchangeByte(0x9f));
	printf("\n 2\t %x",SSP1exchangeByte(0x00));
	printf("\n 3\t %x",SSP1exchangeByte(0x00));
	LPC_GPIO2->FIOSET = (1<<6);
	for ( i = 0; i < 10000; i++ );
	printf("\n******************************************************************************\n");

	//**************************Write to buffer1
	//Opcode for write to buff1
	initSSP1();
	printf("\nWriting to Buffer 1\n");
	LPC_GPIO0->FIOCLR = (1<<6);                         // port 0 pin 6
	SSP1exchangeByte(0x84); //Send 3 Bytes = 24 bits= 16 don't care bits + 8 (1st Byte of buffer) bits
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	//SSP1exchangeByte('0');
	SSP1exchangeByte('S');
	SSP1exchangeByte('J');
	SSP1exchangeByte('S');
	SSP1exchangeByte('U');
	SSP1exchangeByte(' ');
	SSP1exchangeByte('C');
	SSP1exchangeByte('M');
	SSP1exchangeByte('P');
	SSP1exchangeByte('E');
	SSP1exchangeByte('1');
	SSP1exchangeByte('2');
	SSP1exchangeByte('7');
	SSP1exchangeByte(' ');
	SSP1exchangeByte('N');
	SSP1exchangeByte('A');
	SSP1exchangeByte('D');
	SSP1exchangeByte('I');
	SSP1exchangeByte('M');
	SSP1exchangeByte(' ');
	SSP1exchangeByte('S');
	SSP1exchangeByte('A');
	SSP1exchangeByte('R');
	SSP1exchangeByte('R');
	SSP1exchangeByte('A');
	SSP1exchangeByte('S');
	SSP1exchangeByte(' ');
	SSP1exchangeByte('0');
	SSP1exchangeByte('0');
	SSP1exchangeByte('8');
	SSP1exchangeByte('3');
	SSP1exchangeByte('8');
	SSP1exchangeByte('2');
	SSP1exchangeByte('6');
	SSP1exchangeByte('0');
	SSP1exchangeByte('8');

	LPC_GPIO2->FIOSET = (1<<6); // port 2 pin 6
	for ( i = 0; i < 10000; i++ );
	printf("\n******************************************************************************\n");

	initSSP1();
	printf("\nReading from Buffer 1\n");
	LPC_GPIO0->FIOCLR = (1<<6);   // port 0 pin 6
	//**************************Read from buff 1
	//Opcode for Read buff 1
	SSP1exchangeByte(0xd4);
	//Send 3 Bytes = 24 bits= 16 don't care bits + 8 buffer addr bits ????
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);	//trial for buff1
	//Send 1 dummy byte
	int count = 0;
	for(count=0; count<35;count++){
	printf("\n %i\t %c",i,SSP1exchangeByte(0x00));
	}
//	printf("\n 2\t %c",SSP1exchangeByte(0x00));
//	printf("\n 3\t %c",SSP1exchangeByte(0x00));
//	printf("\n 4\t %c",SSP1exchangeByte(0x00));

	printf("\n******************************************************************************\n");
	LPC_GPIO2->FIOSET = (1<<6);
	for ( i = 0; i < 10000; i++ );

	initSSP1();
	/************************************/
	printf("\nWriting from Buffer 1 to Flash Memory\n");
	LPC_GPIO0->FIOCLR = (1<<6);
	//**************************Write from buffer1 to main memory
	//Opcode for write from buffer1 to main memory with built-in erase
	SSP1exchangeByte(0x83);
	//Send 3 Bytes = 24 bits= 4 don't care bits + 12 addr bits (A19-A8) + 8 don't care bits
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	// SSP1exchangeByte(0xff);
	LPC_GPIO2->FIOSET = (1<<6);
	for ( i = 0; i < 10000; i++ );

	//******************************************************************************************************************

	//PART B Verification
	initSSP1();
	printf("\n******************************************************************************\n");
	printf("\nWriting from Flash Memory to Buffer 2\n");
	LPC_GPIO0->FIOCLR = (1<<6);
	//**************************Read from main memory to buff 1
	//Opcode for Read from main memory to buff 1
	SSP1exchangeByte(0x55);
	//Send 3 Bytes = 24 bits= 4 don't care bits + 12 addr bits (A19-A8) + 8 don't care bits
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	LPC_GPIO2->FIOSET = (1<<6);
	for ( i = 0; i < 10000; i++ );
	printf("\n******************************************************************************\n");

	initSSP1();
	printf("\nReading from Buffer 2\n");
	LPC_GPIO0->FIOCLR = (1<<6);
	//**************************Read from buff 1
	//Opcode for Read buff 1
	SSP1exchangeByte(0xd6);
	//Send 3 Bytes = 24 bits= 16 don't care bits + 8 buffer addr bits ????
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);
	SSP1exchangeByte(0x00);	//trial for buff1
	//Send 1 dummy byte
	printf("\n",SSP1exchangeByte(0x00));

	int counter = 0;
	for(counter=0; counter<35;counter++){
	printf("\n %i\t %c",i,SSP1exchangeByte(0x00));
	}
//	printf("\n 1\t %c",SSP1exchangeByte(0x00));
//	printf("\n 2\t %c",SSP1exchangeByte(0x00));
//	printf("\n 3\t %c",SSP1exchangeByte(0x00));
//	printf("\n 4\t %c",SSP1exchangeByte(0x00));
	printf("\n******************************************************************************\n");
	LPC_GPIO2->FIOSET = (1<<6);
	for ( i = 0; i < 10000; i++ );
	//LPC_GPIO0->FIOCLR = (1<<6);


	while(1);
	//delayMs(0,100);
	// printf("\n1\t%x", in);
	//extern void SSPReceive( 1, uint8_t *buf, uint32_t Length );

	return 0;
}
/******************************************************************************
 **                            End Of File
 ******************************************************************************/



