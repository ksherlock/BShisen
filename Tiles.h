#ifndef __TILES_H__
#define __TILES_H__

#include <Bitmap.h>
class Tiles
{
public:
	Tiles(void);
	~Tiles(void);

BBitmap *GetTile(int index) 
{ 
	return tiles[index]; 
};

private:

BBitmap **tiles;

};

#endif
