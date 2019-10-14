//EEE3096S Mini Project A
//RJHINE001 and SMTAMY005

#define BLYNK_PRINT stdout

#ifdef RASPBERRY
#include <blynk-library/linux/BlynkApiWiringPi.h>
#else
#include <blynk-library/linux/BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
#include <blynk-library/src/BlynkWidgets.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>
#include <mcp3004.h>
#include <string.h>
#include <cstdio>
#include <iomanip>

int hexCompensation(int units);
int decCompensation(int units);
void setup(void);
void system_time(void);
void current_time(void);
int main(void);
void timHandler(void);
int startsec;
int startmin;
int starthrs;
int sec;
int min;
int hrs;
int current;
int start;
int sys;
int sys_sec;
int sys_min;
int sys_min_temp;
int sys_hrs;
int RTC_addr=0x6f;
int RTC;
char auth[]="AhPhilHB9QGXB5cEbSVFTJmTJ_3_MtGJ";
BlynkTimer timer;
int main() 
{ 
	///ADC
	printf("wiringPiSPISetup RC=%d\n",wiringPiSPISetup(0,500000));
        mcp3004Setup(100,0);
        double humidity=0;
	setup();
        printf("__________________________________________________________________\n");
        printf("| RTC Time | Sys timer | Humidity | Temp | Light | DAC Out | Alarm |\n");
	// Loop indefinitely, waiting for 100ms between each set of data
        while(1)
        {// read humidity and convert to voltage
		Blynk.run;
		timer.run;
	}
}
void timHandler(void){

		double adcH=analogRead(100);
                humidity = adcH/1023 * 3.3;
                current_time();
                system_time();
                double adcT= analogRead(101);
                double tempV= adcT/1023*3.3;
                double temp=(tempV-0.5)/10;
                //read   light
                double adcl= analogRead(102);
                //calculate DAC output
                double Vout=adcl/1023*humidity
		printf("|%x: %x: %x ",hrs, min, sec );
                printf("|%x: %x: %x      ", sys_hrs, sys_min, sys_sec);
                printf("|");
                std::cout << std::setprecision(4) << humidity;
                printf("       |");
                std::cout << std::setprecision(4) << temp;
                printf("       |");
                std::cout << std::setprecision(4) << adcl;
                printf("       |");
                std::cout << std::setprecision(4) << Vout;
                printf("       |");
                printf("\n");
                //WRITE TO BLYNK
                Blynk.virtualWrite(V1,humidity);
		Blynk.virtualWrite(V2, temperature);
		Blynk.virtualWrite(V3, adcl);
                usleep(1000000);

}

void setup(void) 
{
//set up time
	RTC= wiringPiI2CSetup(RTC_addr);	
	startsec= wiringPiI2CReadReg8(RTC, 0x00)- 0x80;
	startmin=wiringPiI2CReadReg8(RTC, 0x01);
	starthrs=wiringPiI2CReadReg8(RTC, 0x02);
	starthrs=hexCompensation(starthrs)+2;
	starthrs=decCompensation(starthrs);

//setup blynk
	Serial.begin(9600);
	Blynk.begin(auth);
	timer.setInterval(1000L, timHandler);
}

void current_time(void)
{
        sec= wiringPiI2CReadReg8(RTC, 0x00) -0x80;
        min=wiringPiI2CReadReg8(RTC, 0x01);
        hrs=wiringPiI2CReadReg8(RTC, 0x02) ;
   	hrs=hexCompensation(hrs)+2;
        hrs=decCompensation(hrs); 
}


void system_time(void)
{
	current= hexCompensation(sec) + (hexCompensation(min)*60) + ((hexCompensation(hrs))*60*60);
	start= hexCompensation(startsec) + (hexCompensation(startmin) * 60) + (hexCompensation(starthrs) * 60 * 60);
	sys=current-start;
	sys_hrs = decCompensation(sys/(60*60));
	sys_min_temp= sys % (60*60);
	sys_min = decCompensation(sys_min_temp / 60);
	sys_sec= decCompensation(sys_min_temp % 60); 
}
int hexCompensation(int units){
	int unitsU= units%0x10;
	if(units>=0x50){
		units=50+unitsU;}
	else if(units>=0x40){
		units=40+unitsU;}
     else if(units>=0x30){
		units=30+unitsU;}
	else if(units>=0x20){
		units =20+unitsU;}
	else if(units>=0x10){
		units=10+unitsU;}
	return units;}
int decCompensation(int units){
	int unitsU=units%10;
	if (units>= 50)
	{
		units = 0x50 + unitsU;
	}
	else if (units>=40)
	{
		units= 0x40 + unitsU; 
	}
	else if (units >=30)
	{
		units = 0x30 + unitsU;
	}
	else if (units>=20)
	{
		units = 0x20 + unitsU;
	}
	else if (units>=10)
	{
		units = 0x10 + unitsU;
	}
	return units;
	}


