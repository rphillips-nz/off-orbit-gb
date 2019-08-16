#include <gb/gb.h>

typedef struct GameCharacter {
	UBYTE sprite_ids[4]; // character uses 4 sprites
	UINT8 direction;
	UINT8 x;
	UINT8 y;
	BYTE velocity_x;
	BYTE velocity_y;
	UINT8 width;
	UINT8 height;
} GameCharacter;
