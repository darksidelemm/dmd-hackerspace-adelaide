/*
	Freetronics Dot Matrix Display Driver Code - Visual Effects
	
	Authors:	Mark Jessop (vk5qi(at)rfhead(dot)net)
	Date: 2012-05-05
	
	Blah blah, GPLv3 blah blah.

*/

// Do a dissolve effect.
void dissolve(int step_time, int steps){
	for (int i = 0; i<steps; i++){
		random_pixel(0);
		delay(step_time);
	}
	dmd.clearScreen( true );
}

// Set or clear a random pixel on the display.
void random_pixel(int is_on){
	int x = (int)random(32);
	int y = (int)random(16);
	disable_refresh();
	dmd.writePixel(x,y,GRAPHICS_NORMAL,is_on);
	enable_refresh();
}
