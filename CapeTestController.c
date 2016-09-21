/*	Switch LED Cape Controller
	Based on Molloy's Listing 13-3, ledButton.c
	GPIO P8.12 input (switch)
	GPIO P8,10 output (LED)
	09/21/2016
*/

#include <errno.h>
#include <fcntl.h>
//#include <pool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio/"
#define POLL_TIMEOUT (3 * 1000)							// 3 seconds
#define MAX_BUF 64

enum GPIO_DIRECTION	{INPUT, OUTPUT};
enum GPIO_VALUE		{OFF, ON};

int gpioExport(unsigned int gpio);
int gpioUnexport(unsigned int gpio);
int gpioSetDir(unsigned int gpio, unsigned int out_flag);
int gpioSetValue(unsigned int gpio, unsigned int value);
int gpioGetValue(unsigned int gpio, unsigned int *value);
int gpioSetEdge(unsigned int gpio, char *edge);
int gpioFdOpen(unsigned int gpio);
int gpioFdClose(int fd);

//______________________________________
int main (void)
{
	int i, n;
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 500000000L;

	if (getuid()!=0)
	{
		printf("You must run this program as root! Exiting.\n");
		exit(EXIT_FAILURE);
	}

	// Initialize Controller GPIO: P8.12 = input, P8.10 = output
	gpioExport(44);										// P8.12 = gpio1[12] = 1*32+12 = 44
	gpioSetDir(44, INPUT);

	gpioExport(68);						// P8.10 = gpio2[04] = 2*32+ 4 = 68
	gpioSetDir(68, OUTPUT);
	gpioSetValue(68, OFF);

	// Initialize structure used by prussdrv_pruintc_intc
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

	// Allocate and initialize memory
	prussdrv_init();
	prussdrv_open(PRU_EVTOUT_0);

	// Map PRU's interrupts
	prussdrv_pruintc_init(&pruss_intc_initdata);

	// Set up pointers to PRU data memory
//	prussdrv_map_prumem(PRUSS0_PRU1_DATARAM, &pru1DataMemory);

	prussdrv_exec_program(0, "./CapeTestPru.bin");		// load and execute PRU 0 program

	printf("CapeTestController running...\n");

	// Blink Controller LED a couple of times
	for (i=0; i<10; i++)
	{
		gpioSetValue(68, ON);
		nanosleep(&tim, NULL);
		gpioSetValue(68, OFF);
		nanosleep(&tim, NULL);
	}
	gpioSetValue(68, ON);								// LED on till PRU is done

	// Wait for event completion from PRU
	n = prussdrv_pru_wait_event(PRU_EVTOUT_0);
	printf("PRU indicating program complete; event number= %d\n", n);
	gpioSetValue(68, OFF);

//	printf("---Shutting down...\n");

	prussdrv_pru_disable(0);							// disable PRU and close memory mappings
	prussdrv_exit();
	return EXIT_SUCCESS;
}

//______________________________________
int gpioExport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/export");
		return fd;
	}
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

//______________________________________
int gpioUnexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/export");
		return fd;
	}
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

//______________________________________
int gpioSetDir(unsigned int gpio, unsigned int out_flag)
{
	int fd;
	char buf[MAX_BUF];
 
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/direction");
		return fd;
	}

	if (out_flag)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);

	close(fd);
	return 0;
}

//______________________________________
int gpioSetValue(unsigned int gpio, unsigned int value)
{
	int fd;
	char buf[MAX_BUF];
 
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/set-value");
		return fd;
	}

	if (value)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);

	close(fd);
	return 0;
}

//______________________________________
int gpioGetValue(unsigned int gpio, unsigned int *value)
{
	int fd;
	char buf[MAX_BUF];
	char ch;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0)
	{
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	return 0;
}

//______________________________________
int gpioSetEdge(unsigned int gpio, char *edge)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0)
	{
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1); 
	close(fd);
	return 0;
}

//______________________________________
int gpioFd_Open(unsigned int gpio)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0)
		perror("gpio/fd_open");

	return fd;
}

//______________________________________
int gpioFdClose(int fd)
{
	return close(fd);
}
