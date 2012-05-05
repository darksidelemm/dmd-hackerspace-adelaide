/*
	Freetronics Dot Matrix Display Driver Code
	
	Based off the dmd_demo example provided by freetronics.
	
	Authors:	Mark Jessop (vk5qi(at)rfhead(dot)net)
	Date: 2012-05-05
	
	
	IMPORTANT: This code requires modification of the DMD library.
	You will need to move the bDMDScreenRAM declaration OUT of the private area, so we can
	write to it directly.
	
	
	Blah blah, GPLv3 blah blah.

*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD(){ 
  dmd.scanDisplayBySPI();
}

// Interrupt enable/disable helper functions, in an attempt to reduce flicker
// by stopping concurrent access. Doesn't work.
void disable_refresh(){
	Timer1.detachInterrupt();
}
void enable_refresh(){
	Timer1.attachInterrupt( ScanDMD ); 
}

void setup(void){
	// Set a seed value for the random number stuff we might be using later.
	randomSeed(analogRead(0));

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
}



// Pixellated Hackerspace Adelaide Logo. Sorry Simon :-(
static byte ha_logo[] = {
   0xfc, 0x01, 0xe0, 0x03, 0x54, 0x01, 0xf0, 0x07, 0x54, 0x01, 0x38, 0x0e,
   0x54, 0x01, 0xdc, 0x1d, 0x54, 0x01, 0xac, 0x1a, 0x54, 0x03, 0xa6, 0x32,
   0x54, 0x0f, 0xde, 0x3d, 0xd4, 0x1e, 0x6e, 0x3b, 0xb4, 0x3d, 0xb6, 0x36,
   0x74, 0x7b, 0xb6, 0x36, 0x74, 0x77, 0xda, 0x2d, 0x74, 0x77, 0xda, 0x2d,
   0x74, 0x77, 0xda, 0x2d, 0x74, 0x77, 0xda, 0x2d, 0x74, 0x77, 0xda, 0x2d,
   0xfc, 0x7f, 0xfe, 0x3f };
   
   
   
void loop(void){
	dmd.clearScreen( true );
	
	// Scrolling Marquee
	dmd.selectFont(Arial_Black_16);
	dmd.drawMarquee("Hackerspace is in session!",26,(32*DISPLAYS_ACROSS)-1,0);
	long start=millis();
	long timer=start;
	boolean ret=false;
	while(!ret){
		if ((timer+30) < millis()) {
			ret=dmd.stepMarquee(-1,0);
			timer=millis();
		}
	}

	// Draw Hackerspace Adelaide Logo
	write_xbitmap(ha_logo,64); 
	delay(5000);
	
	// Run Conway's Game of Life over the logo for 100 iterations.
	for(int i = 0; i<100; i++){
		GOL_run_step();
		delay(50); // Small delay.
	}
	
	// Dissolve to black.
	dissolve(1,2000); //1ms period, 2000 steps - should take 2 seconds.
}




