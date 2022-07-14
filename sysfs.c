/*
  Luciano Regis Orlandini - 460952
  ETI2v.IA

  sysfs 7-Seg programme in C.
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1
#define VALUE_MAX 30

#define GPIO0 17
#define GPIO1 18 
#define GPIO2 27 
#define GPIO3 22 
#define GPIO4 23 
#define GPIO5 24 
#define GPIO6 25 
#define GPIO7 4  

static int GPIOExport(int pin) {
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir) {
	static const char s_directions_str[]  = "in\0out";
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

static int GPIOWrite(int pin, int value) {
	static const char s_values_str[] = "01";
	char path[VALUE_MAX];
	int fd;
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
	close(fd);
	return(0);
}

// Clears 7-Seg display
void clearDisplay() {
  for (int index = 0; index < 8; index++) {
    GPIOWrite(GPIO0, 0);
    GPIOWrite(GPIO1, 0);
    GPIOWrite(GPIO2, 0);
    GPIOWrite(GPIO3, 0);
    GPIOWrite(GPIO4, 0);
    GPIOWrite(GPIO5, 0);
    GPIOWrite(GPIO6, 0);
    GPIOWrite(GPIO7, 0);
  }
}

// Lights up 7-Seg according to parameter input
void lightDisplay(int a, int b, int c, int d, int e, int f, int g, int dp) {
    GPIOWrite(GPIO0, a);
    GPIOWrite(GPIO1, b);
    GPIOWrite(GPIO2, c);
    GPIOWrite(GPIO3, d);
    GPIOWrite(GPIO4, e);
    GPIOWrite(GPIO5, f);
    GPIOWrite(GPIO6, g);
    GPIOWrite(GPIO7, dp);
}

// Defines 7-Seg values
void sevenSeg(char c) {
  switch(c) {
    case '0':
      lightDisplay(1, 1, 1, 1, 1, 1, 0, 0);
      break;
    case '1':
      lightDisplay(0, 1, 1, 0, 0, 0, 0, 0);
      break;
    case '2':
      lightDisplay(1, 1, 0, 1, 1, 0, 1, 0);
      break;
    case '3':
      lightDisplay(1, 1, 1, 1, 0, 0, 1, 0);
      break;
    case '4':
      lightDisplay(0, 1, 1, 0, 0, 1, 1, 0);
      break;
    case '5':
      lightDisplay(1, 0, 1, 1, 0, 1, 1, 0);
      break;
    case '6':
      lightDisplay(1, 0, 1, 1, 1, 1, 1, 0);
      break;
    case '7':
      lightDisplay(1, 1, 1, 0, 0, 0, 0, 0);
      break;
    case '8':
      lightDisplay(1, 1, 1, 1, 1, 1, 1, 0);
      break;
    case '9':
      lightDisplay(1, 1, 1, 1, 0, 1, 1, 0);
      break;
    case 'A':
      lightDisplay(1, 1, 1, 0, 1, 1, 1, 0); 
      break;
    case 'B':
      lightDisplay(0, 0, 1, 1, 1, 1, 1, 0); 
      break;
    case 'C':
      lightDisplay(1, 0, 0, 1, 1, 1, 0, 0); 
      break;
    case 'D':
      lightDisplay(0, 1, 1, 1, 1, 0, 1, 0); 
      break;
    case 'E':
      lightDisplay(1, 0, 0, 1, 1, 1, 1, 0); 
      break;
    case 'F':
      lightDisplay(1, 0, 0, 0, 1, 1, 1, 0); 
      break;
    default:
      lightDisplay(0, 0, 0, 0, 0, 0, 0, 1);
      break;
  }
} 

int main(int argc, char *argv[]) {
	
	// Enables GPIO pins
	GPIOExport(GPIO0);
	GPIOExport(GPIO1);
	GPIOExport(GPIO2);
	GPIOExport(GPIO3);
	GPIOExport(GPIO4);
	GPIOExport(GPIO5);
	GPIOExport(GPIO6);
	GPIOExport(GPIO7);
	
	// Sets GPIO direction
	GPIODirection(GPIO0, OUT);
	GPIODirection(GPIO1, OUT);
	GPIODirection(GPIO2, OUT);
	GPIODirection(GPIO3, OUT);
	GPIODirection(GPIO4, OUT);
	GPIODirection(GPIO5, OUT);
	GPIODirection(GPIO6, OUT);
	GPIODirection(GPIO7, OUT);

	if (argc > 1) {
	int numArg = argc - 1;
	size_t argLen;
	for (int index = 1; index <= numArg; index++) {
	  argLen = strlen(argv[index]);
	  char* tempArgv = argv[index];
	  for (int i = 0; i < argLen; i++) {
		sevenSeg(toupper(tempArgv[i]));
		usleep(500000);
		clearDisplay();
		usleep(250000);
	  }
	  if (numArg > 1) {
		sevenSeg(' ');
		sleep(1);
	  } 
	}
	} else {
	printf("At least 1 command argument is expected.\n");
	}
	clearDisplay(); 

	// Disables GPIO pins
	GPIOUnexport(GPIO0);
	GPIOUnexport(GPIO1);
	GPIOUnexport(GPIO2);
	GPIOUnexport(GPIO3);
	GPIOUnexport(GPIO4);
	GPIOUnexport(GPIO5);
	GPIOUnexport(GPIO6);
	GPIOUnexport(GPIO7);
	
	return(0);
}
