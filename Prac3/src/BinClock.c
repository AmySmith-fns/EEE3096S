/*
 * BinClock.c
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019
 * 
 * RJHINE001 SMTAMY005
 * 10/08/2019
*/

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <signal.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions
#include <math.h>
#include "BinClock.h"
#include "CurrentTime.h"

//Global variables
int hours, mins, secs;
long lastInterruptTime = 0; //Used for button debounce
int RTC; //Holds the RTC instance

int HH,MM,SS;
const int HLED[]={0,2,3,25};
const int MLED[]={7,22,21,27,4,6};
void initGPIO(void){
	/* 
	 * Sets GPIO using wiringPi pins. see pinout.xyz for specific wiringPi pins
	 * You can also use "gpio readall" in the command line to get the pins
	 * Note: wiringPi does not use GPIO or board pin numbers (unless specifically set to that mode)
	 */
	printf("Setting up\n");
	wiringPiSetup(); //This is the default mode. If you want to change pinouts, be aware
	
	RTC = wiringPiI2CSetup(RTCAddr); //Set up the RTC
	
	//Set up the LEDS
	for(int i=0; i < sizeof(LEDS)/sizeof(LEDS[0]); i++){
	    pinMode(LEDS[i], OUTPUT);
	}
	
	//Set Up the Seconds LED for PWM
	pinMode(1,PWM_OUTPUT);
	
	printf("LEDS done\n");
	
	//Set up the Buttons
	for(int j=0; j < sizeof(BTNS)/sizeof(BTNS[0]); j++){
		pinMode(BTNS[j], INPUT);
		pullUpDnControl(BTNS[j], PUD_UP);
		printf("LED done");
	}

	// set the button interrupts
	wiringPiISR (5, INT_EDGE_FALLING,hourInc);
	wiringPiISR (30, INT_EDGE_FALLING, minInc);
	
	
	printf("BTNS done\n");
	printf("Setup done\n");
}


/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
int main(void){
	initGPIO();
	//Set random time (3:04PM)
	
	wiringPiI2CWriteReg8(RTC, HOUR, 0x1);
	wiringPiI2CWriteReg8(RTC, MIN, 0x3);
	wiringPiI2CWriteReg8(RTC, SEC, 0b10000000); 
	
	// Repeat this until we shut down
	for (;;){
		//Fetch the time from the RTC
		SS=wiringPiI2CReadReg8(RTC, SEC);
		MM=wiringPiI2CReadReg8(RTC, MIN);
		HH=wiringPiI2CReadReg8(RTC, HOUR);
		//Function calls to toggle LEDs
		SS=SS-0x80;
		
              //convert digital hours to analogue   
                HH= hFormat(HH);
		
		//turn on LEDS
		lightMins(MM);
		lightHours(HH);
		// put seconds on pwm
		secPWM(SS);
		
		// Print out the time we have stored on our RTC	
		printf("The current time is: %x:%x:%x\n", HH, MM, SS);
		
		//using a delay to make our program "less CPU hungry"
		delay(900); //milliseconds
	}
	return 0;
}

/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours){
	/*formats to 12h*/
	//convert to decimal first
	hours=hexCompensation(hours);
	
	if (hours >= 24){
		hours = 0;
	}
	else if (hours > 12){
		hours -= 12;
	}
	return (int)hours;
}

/*
 * Turns on corresponding LED's for hours
 */
void lightHours(int units){
	
	int temp = units;
	int rem=0;
	int bin[]={0,0,0,0};
	//convert to binary and store in bin
	for (int i=0; i<=3; i++){
		rem= temp%2;
		temp=temp/2;
		bin[i]=rem;
	}
	//write bin to LEDS
	for (int i=0; i<=3;i++){
		digitalWrite(HLED[i], bin[i]);
	}
	
}

/*
 * Turn on the Minute LEDs
 */
void lightMins(int units){
	
        int temp = units;
        int rem=0;
        int bin[]={0,0,0,0,0,0};
	//convert to binary and store in bin
        for (int i=0; i<=5; i++){
                rem= temp%2;
                temp=temp/2;
                bin[i]=rem;}
	//output bin on LEDS
        for (int i=0; i<=5;i++){
                digitalWrite(MLED[i], bin[i]);
        }
}

/*
 * PWM on the Seconds LED
 * The LED should have 60 brightness levels
 * The LED should be "off" at 0 seconds, and fully bright at 59 seconds
 */
void secPWM(int units){
	//100/60=1.67% per second.
	// 1.67*SS= output Duty cycle
	int out = hexCompensation(units);
	out=1.67*out;
	printf("%d", out);
	pwmWrite(1,out);
}

/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;

	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
		
	
		
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


/*
 * hourInc
 * Fetch the hour value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 23 hours in a day
 * Software Debouncing should be used
 */
void hourInc(void){
	//Debounce
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 1 triggered, %x\n", hours);
		       //Fetch RTC Time
      
                HH=wiringPiI2CReadReg8(RTC, HOUR);
		HH= hexCompensation(HH); //convert hex to decimal
                //Increase hours by 1, ensuring not to overflow
                if(HH==12){//set hours back to 1 if they reach 12
                        HH=1;
                        }
                else HH+=1;
		HH=decCompensation(HH);//convert decimal back to hex
                //Write hours back to the RTC
       		wiringPiI2CWriteReg8(RTC, HOUR, HH);
     
        }
	
	lastInterruptTime = interruptTime;
}

/* 
 * minInc
 * Fetch the minute value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 60 minutes in an hour
 * Software Debouncing should be used
 */
void minInc(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 2 triggered, %x\n", mins);
		//Fetch RTC Time
 	        MM=wiringPiI2CReadReg8(RTC, MIN);
		//convert hex to decimal
        	MM=hexCompensation(MM);
		//Increase minutes by 1, ensuring not to overflow
		if(MM==59){ //if minutes reach 59, set back to 0
			MM=0;
			}
		else MM=MM+1;
	// convert decimal back to hex
		MM=decCompensation(MM);
		//Write minutes back to the RTC
	
        wiringPiI2CWriteReg8(RTC, MIN, MM);
        
	}
	lastInterruptTime = interruptTime;
}

//This interrupt will fetch current time from another script and write it to the clock registers
//This functions will toggle a flag that is checked in main
void toggleTime(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		HH = getHours();
		MM = getMins();
		SS = getSecs();

		HH = hFormat(HH);
		HH = decCompensation(HH);
		wiringPiI2CWriteReg8(RTC, HOUR, HH);

		MM = decCompensation(MM);
		wiringPiI2CWriteReg8(RTC, MIN, MM);

		SS = decCompensation(SS);
		wiringPiI2CWriteReg8(RTC, SEC, 0b10000000+SS);

	}
	lastInterruptTime = interruptTime;
}
