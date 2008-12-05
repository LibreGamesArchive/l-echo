
//{{BLOCK(topscreen)

//======================================================================
//
//	topscreen, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 325 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 20800 + 1536 = 22848
//
//	Time-stamp: 2008-10-26, 17:42:31
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __TOPSCREEN__
#define __TOPSCREEN__

#define topscreenTilesLen 20800
extern const unsigned short topscreenTiles[10400];

#define topscreenMapLen 1536
extern const unsigned short topscreenMap[768];

#define topscreenPalLen 512
extern const unsigned short topscreenPal[256];

#endif // __TOPSCREEN__

//}}BLOCK(topscreen)
