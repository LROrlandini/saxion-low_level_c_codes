/*
  Luciano Regis Orlandini - 460952
  ETI2v.IA

  WiringPi 7-Seg programme in C.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <wiringPi.h>

#define DELAY 400

// Sets up so all 7-seg pins are set as output
void pinSetup() {
  for (int index = 0; index < 8; index++) {
    pinMode(index, OUTPUT);
  } 
}

// Clears 7-Seg display
void clearDisplay() {
  for (int index = 0; index < 8; index++) {
    digitalWrite(index, 0);
  }
}

// Lights up 7-Seg according to parameter input
void lightDisplay(int a, int b, int c, int d, int e, int f, int g, int dp) {
  int segments[] = {a, b, c, d, e, f, g, dp};
  for (int index = 0; index < 8; index++) {
    digitalWrite(index, segments[index]);
  }
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

// Main running of programme
int main (int argc, char * argv[])
{
  printf("Luciano Orlandini WiringPi\n");
  if(wiringPiSetup() == -1) {
    return 1 ;
  }
  pinSetup();
  if (argc > 1) {
    int numArg = argc - 1;
    size_t argLen;
    for (int index = 1; index <= numArg; index++) {
      argLen = strlen(argv[index]);
      char* tempArgv = argv[index];
      for (int i = 0; i < argLen; i++) {
        sevenSeg(toupper(tempArgv[i]));
        delay(DELAY);
        clearDisplay();
        delay(DELAY / 2);
      }
      if (numArg > 1) {
        sevenSeg(' ');
        delay(DELAY * 4);
      } 
    }
  } else {
    printf("At least 1 command argument is expected.\n");
  }
  clearDisplay();  
  return 0;
}

