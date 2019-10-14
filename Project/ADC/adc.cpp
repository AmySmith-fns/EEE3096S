#include <string.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <wiringPiSPI.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#define BASE 100
#define SPI_CHAN 0
int main (int argc, char *argv[])
{printf("wiringPiSPISetup RC=%d\n",wiringPiSPISetup(0,500000));
	mcp3004Setup(BASE,SPI_CHAN);
	int humidity=0;
	printf("__________________________________________________________________\n");
	printf("RTC Time | Sys timer | Humidity | Temp | Light | DAC Out | Alarm |\n");
// Loop indefinitely, waiting for 100ms between each set of data
	while(1)
	{// read humidity and convert to voltage
		double adcH= analogRead(100);	
		double humidity= adcH/1023*3.3;
		// read temp and convert to a voltage. calculate ambient temp from Vtemp
		double adcT= analogRead(101);
		double tempV= adcT/1023*3.3;
		double temp=(adcT-0.5)/10;
		//read   light
		double adcl= analogRead(102);
		//calculate DAC output
		double Vout=adcl/1023*humidity;
		std::cout << std::setprecision(4) << humidity;
		
		printf("\n");
		std::cout << std::setprecision(4) << tempV;
		printf("\n");
		std::cout << std::setprecision(4) << temp;
		printf("\n");
		printf("\n");
		usleep(1000000);
	}
}
