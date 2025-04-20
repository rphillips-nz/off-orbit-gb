#ifndef _OO_GAME_CHARACTER_H_
#define _OO_GAME_CHARACTER_H_

#include <gb/gb.h>

typedef struct GameCharacter {
	UBYTE sprite_ids[4];
	UBYTE sprite_ids_count;
	UBYTE is_alternate;
	UBYTE is_destroyed;
	UBYTE is_destroying;
	UBYTE frame;
	uint16_t last_animated_at;
	uint8_t direction;
	uint8_t x;
	uint8_t y;
	int8_t velocity_x;
	int8_t velocity_y;
	uint8_t width;
	uint8_t height;
	uint8_t half_width; // saves calculating this all the time
	uint8_t half_height; // saves calculating this all the time
} GameCharacter;

void destroy_game_character(GameCharacter* character);

void render_game_character(GameCharacter* character);

void update_game_character(GameCharacter* character, GameCharacter* player);

#endif
