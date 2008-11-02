
//{{BLOCK(menu)

//======================================================================
//
//	menu, 256x192@8, 
//	+ palette 256 entries, not compressed
//	+ 49 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x24 
//	Total size: 512 + 3136 + 1536 = 5184
//
//	Time-stamp: 2008-06-29, 21:05:07
//	Exported by Cearn's GBA Image Transmogrifier
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef __MENU__
#define __MENU__

#define menuTilesLen 3136
extern const unsigned int menuTiles[784];

#define menuMapLen 1536
extern const unsigned short menuMap[768];

#define menuPalLen 512
extern const unsigned short menuPal[256];

#endif // __MENU__

//}}BLOCK(menu)
