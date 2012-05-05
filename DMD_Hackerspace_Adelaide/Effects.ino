/*
	Freetronics Dot Matrix Display Driver Code - Visual Effects
	
	Authors:	Mark Jessop (vk5qi(at)rfhead(dot)net)
	Date: 2012-05-05
	
	Blah blah, GPLv3 blah blah.

*/

void dissolve(int step_time, int steps){
	for (int i = 0; i<steps; i++){
		blank_random_pixel();
		delay(step_time);
	}
	dmd.clearScreen( true );
}


void blank_random_pixel(){
	int x = (int)random(32);
	int y = (int)random(16);
	disable_refresh();
	dmd.writePixel(x,y,GRAPHICS_NORMAL,0);
	enable_refresh();
}