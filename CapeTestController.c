/*	Switch LED Cape Controller
	Based on Molloy's Listing 13-3, ledButton.c
	GPIO P8.12 input (switch)
	GPIO P8,10 output (LED)
	09/20/2016
*/

//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

//______________________________________
int main (void)
{
	int i, n;

	if (getuid()!=0)
	{
		printf("You must run this program as root! Exiting.\n");
		exit(EXIT_FAILURE);
	}

	// Initialize Controller GPIO
	

	// Initialize structure used by prussdrv_pruintc_intc
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

	// Allocate and initialize memory
	prussdrv_init();
	prussdrv_open(PRU_EVTOUT_0);

	// Map PRU's interrupts
	prussdrv_pruintc_init(&pruss_intc_initdata);

	// Set up pointers to PRU data memory
//	prussdrv_map_prumem(PRUSS0_PRU1_DATARAM, &pru1DataMemory);

	prussdrv_exec_program(0, "./CapeTestPru.bin");			// load and execute PRU 0 program

	printf("CapeTestController running...\n");

	// Blink Controller LED a couple of times
	for (i=0; i<10; i++)
	{
		pin_high(8,10);
		iolib_delay_ms(500);
		pin_low(8, 10);
		iolib_delay_ms(500);
	}
	pin_high(8,10);				// LED on till PRU is done

	// Wait for event completion from PRU
	n = prussdrv_pru_wait_event(PRU_EVTOUT_0);
	printf("PRU indicating program complete; event number= %d\n", n);
	pin_low(8,10);

//	printf("---Shutting down...\n");

	prussdrv_pru_disable(0);				// disable PRU and close memory mappings
	prussdrv_exit();
	iolib_free();
	return EXIT_SUCCESS;
}
