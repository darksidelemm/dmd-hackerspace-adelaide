/*
	Freetronics Dot Matrix Display Driver Code - Game of Life Implementation
	
	Authors:	Mark Jessop (vk5qi(at)rfhead(dot)net)
	Date: 2012-05-05
	
	Blah blah, GPLv3 blah blah.

*/

// This is ONLY going to work with one screen at the moment.. I think.

#define GRID_X	32
#define GRID_Y	16

// The Grid (one day, I got in)
byte GOL_grid[GRID_X/8][GRID_Y][2]; //We compress the X rows into bytes. Saves memory!

//lookup table for DMD::writePixel to make the pixel indexing routine faster
static byte golPixelLookupTable[8] =
{
   0x80,   //0, bit 7
   0x40,   //1, bit 6
   0x20,   //2. bit 5
   0x10,   //3, bit 4
   0x08,   //4, bit 3
   0x04,   //5, bit 2
   0x02,   //6, bit 1
   0x01    //7, bit 0
};

// Blanks out a page.
void GOL_blank_grid(int page){
	for(int x = 0; x<GRID_X/8; x++){
		for(int y = 0; y<GRID_Y; y++){
			GOL_grid[x][y][page] = 0;
		}
	}
}

// The following functions import and export data from the DMD framebuffers.
void GOL_import_framebuffer(int page){
	int index = 0;
	for(int y = 0; y<GRID_Y; y++){
		for (int x = 0; x<(GRID_X/8); x++){
			GOL_grid[x][y][page] = ~dmd.bDMDScreenRAM[index]; // We need to invert the framebuffer data?!
			index++;
		}
	}
}

void GOL_export_framebuffer(int page){
	int index = 0;
	for(int y = 0; y<GRID_Y; y++){
		for (int x = 0; x<(GRID_X/8); x++){
			dmd.bDMDScreenRAM[index] = GOL_grid[x][y][page]; // But not when we write it back.. aaargh.
			index++;
		}
	}
}

// Abstraction functions, to read/write a cell.

int GOL_read_cell(int x, int y, int page){
	// For the moment, we'll say that anything outside the edge of the board is a dead cell. Might look at this later.
	if(x<0 || y<0) return 0;
	if(x>= GRID_X || y>=GRID_Y) return 0;
	
	byte grid_byte = GOL_grid[x/8][y][page];
	//Serial.print("test: ");
	//Serial.println(grid_byte,BIN);
	return (grid_byte&golPixelLookupTable[x%8])>0;
}

void GOL_write_cell(int x, int y, int page, int val){
	byte grid_byte = GOL_grid[x/8][y][page];
	if(val==true){
		grid_byte &= ~golPixelLookupTable[x%8];
	}else{
		grid_byte |= golPixelLookupTable[x%8];
	}
	GOL_grid[x/8][y][page] = grid_byte;
}


// Conways Game of Life Stuff.

// Get the number of neighbours.
int GOL_get_neighbours(int x, int y, int page){
	return	GOL_read_cell(	(x+1),	y, 	page 	) + 
			GOL_read_cell(	x,	(y+1),	page	) + 
			GOL_read_cell( (x-1),	y,	page	) +
			GOL_read_cell( 	x,	(y-1),	page	) +
			GOL_read_cell(	(x+1), (y+1), page	) +
			GOL_read_cell(	(x-1), (y+1), page	) + 
			GOL_read_cell(  (x-1), (y-1), page	) +
			GOL_read_cell(	(x+1), (y-1), page	);
}	

void GOL_run_step(){
	//Read in the framebuffer.
	GOL_import_framebuffer(0);
	
	for(int x=0; x<GRID_X; x++){
		for(int y=0; y<GRID_Y; y++){
			GOL_write_cell(x,y,1,GOL_read_cell(x,y,0));
		
			byte current_cell = GOL_read_cell(x,y,0);
			
			int count = GOL_get_neighbours(x,y,0);
			
			if(count == 3 && current_cell == 0){
				GOL_write_cell(x,y,1,1);
			}
			if( (count<2 || count>3) && current_cell == 1) {
				GOL_write_cell(x,y,1,0);
			}
		}
	}
	GOL_export_framebuffer(1);
}

