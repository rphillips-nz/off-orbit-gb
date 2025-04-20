#include <gb/gb.h>
#include "player.h"
#include "game-character.h"
#include "constants.h"

extern GameCharacter player;
extern UBYTE next_sprite_index;
extern UBYTE game_running;

void reset_player(void) {
	player.x = 80u;
	player.y = 80u;
	player.velocity_x = 0;
	player.velocity_y = 0;
	player.frame = 0u;
	player.is_destroying = 0u;
	player.is_destroyed = 0u;
	animate_player(J_UP);
}

void set_up_player(void) {
	reset_player();
	player.width = 16u;
	player.height = 16u;
	player.half_width = 8u;
	player.half_height = 8u;
	player.sprite_ids_count = 4u;

	set_sprite_tile(next_sprite_index, 0u);
	player.sprite_ids[0u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1u);
	player.sprite_ids[1u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2u);
	player.sprite_ids[2u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3u);
	player.sprite_ids[3u] = next_sprite_index++;
}

void turn_player(uint8_t jpad) {
	if (jpad & J_LEFT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 12u);
		set_sprite_tile(player.sprite_ids[1u], 13u);
		set_sprite_tile(player.sprite_ids[2u], 14u);
		set_sprite_tile(player.sprite_ids[3u], 15u);
	}

	if (jpad & J_RIGHT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 8u);
		set_sprite_tile(player.sprite_ids[1u], 9u);
		set_sprite_tile(player.sprite_ids[2u], 10u);
		set_sprite_tile(player.sprite_ids[3u], 11u);
	}

	if (jpad & J_UP) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 0u);
		set_sprite_tile(player.sprite_ids[1u], 1u);
		set_sprite_tile(player.sprite_ids[2u], 2u);
		set_sprite_tile(player.sprite_ids[3u], 3u);
	}

	if (jpad & J_DOWN) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 4u);
		set_sprite_tile(player.sprite_ids[1u], 5u);
		set_sprite_tile(player.sprite_ids[2u], 6u);
		set_sprite_tile(player.sprite_ids[3u], 7u);
	}
}

void destroy_player(void) {
	player.is_destroyed = 1u;
	player.is_destroying = 0u;
	player.frame = 0u;

	// Hides player sprites
	player.x = 0u;
	player.y = 0u;
	render_game_character(&player);
	player.x = 80u;
	player.y = 80u;

	game_running = 0u;
}

void start_destroying_player(void) {
	player.is_destroying = 1u;
}

void animate_player(uint8_t jpad) {
	switch (player.frame) {
		case 0u:
			turn_player(jpad);
			break;
		case 1u:
			set_sprite_tile(player.sprite_ids[0u], 44u);
			set_sprite_tile(player.sprite_ids[1u], 45u);
			set_sprite_tile(player.sprite_ids[2u], 46u);
			set_sprite_tile(player.sprite_ids[3u], 47u);
			break;
		case 2u:
			set_sprite_tile(player.sprite_ids[0u], 48u);
			set_sprite_tile(player.sprite_ids[1u], 49u);
			set_sprite_tile(player.sprite_ids[2u], 50u);
			set_sprite_tile(player.sprite_ids[3u], 51u);
			break;
		case 3u:
			set_sprite_tile(player.sprite_ids[0u], 52u);
			set_sprite_tile(player.sprite_ids[1u], 53u);
			set_sprite_tile(player.sprite_ids[2u], 54u);
			set_sprite_tile(player.sprite_ids[3u], 55u);
			break;
		case 4u:
			set_sprite_tile(player.sprite_ids[0u], 56u);
			set_sprite_tile(player.sprite_ids[1u], 57u);
			set_sprite_tile(player.sprite_ids[2u], 58u);
			set_sprite_tile(player.sprite_ids[3u], 59u);
			break;
		default:
			destroy_player();
			break;
	}
}

void update_player(uint8_t jpad) {
	if (player.is_destroying && (sys_time - player.last_animated_at > ANIMATION_RATE)) {
		player.frame++;
		player.last_animated_at = sys_time;
	}

	animate_player(jpad);
}
