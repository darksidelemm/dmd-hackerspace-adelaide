/*
	Freetronics Dot Matrix Display Driver Code - XBitmap painter.
	
	Authors:	Mark Jessop (vk5qi(at)rfhead(dot)net)
	Date: 2012-05-05
	
	Blah blah, GPLv3 blah blah.

*/

// Write an XBitmap to the frame buffer.   
void write_xbitmap(byte* data, int length){
	for(int i = 0; i<length; i++){
		dmd.bDMDScreenRAM[i] = ~reverse_byte(data[i]); // Need to invert the byte written to the buffer.
	}
}

// X-Bitmaps data bytes are reversed, need to reverse them to display.
byte reverse_byte(byte c){
	c = (c & 0x0F) << 4 | (c & 0xF0) >> 4;
	c = (c & 0x33) << 2 | (c & 0xCC) >> 2;
	c = (c & 0x55) << 1 | (c & 0xAA) >> 1;
	return c;
}