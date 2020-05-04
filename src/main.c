#include <gb/gb.h>

#include "sound/gbt_player.h"
#include "sound/sound.h"
#include "constants.h"
#include "game-character.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"

#include "fade.c"
#include "font.c"
#include "graphics/sprite-data.c"
#include "graphics/background-data.c"
#include "graphics/background-map.c"
#include "graphics/title-data.c"
#include "graphics/title-map.c"

extern const unsigned char* song_Data[];

GameCharacter player;
GameCharacter bullets[BULLET_COUNT];
GameCharacter enemies[ENEMY_COUNT];

UBYTE game_running;
UBYTE next_sprite_index;
UBYTE score;
UBYTE health;

const UINT8 reset_prompt_map[11u] = {
	BACKGROUND_DATA_TILES + 26u, // P
	BACKGROUND_DATA_TILES + 28u, // R
	BACKGROUND_DATA_TILES + 15u, // E
	BACKGROUND_DATA_TILES + 29u, // S
	BACKGROUND_DATA_TILES + 29u, // S
	0u,
	BACKGROUND_DATA_TILES + 29u, // S
	BACKGROUND_DATA_TILES + 30u, // T
	BACKGROUND_DATA_TILES + 11u, // A
	BACKGROUND_DATA_TILES + 28u, // R
	BACKGROUND_DATA_TILES + 30u  // T
};

const UINT8 heart_map[1u] = {BACKGROUND_DATA_TILES}; // The heart tile
const UINT8 blank_map[1u] = {0u};

UBYTE did_collide(GameCharacter* a, GameCharacter* b) {
	if (a->is_destroyed || b->is_destroyed || a->is_destroying || b->is_destroying) {
		return 0u;
	}

	return !(
		(a->y + a->height) < b->y || // a_bottom < b_top
		a->y > (b->y + b->height) || // a_top > b_bottom
		(a->x + a->width) < b->x ||  // a_right < b_left
		a->x > (b->x + b->width)     // a_left > b_right
	);
}

void update_score() {
	UINT8 digit_map[1u];
	UBYTE score_iterator = score;
	INT8 digit_render_count = 0u;

	do {
		digit_map[0u] = score_iterator % 10u + BACKGROUND_DATA_TILES + 1u;
		set_win_tiles(19u - digit_render_count, 0u, 1u, 1u, digit_map);
		digit_render_count++;
		score_iterator /= 10u;
	} while (score_iterator > 0u);
}

void update_health() {
	UBYTE i;

	for (i = 0u; i < MAX_HEALTH; i++) {
		set_win_tiles(i, 0u, 1u, 1u, i > health - 1 ? blank_map : heart_map);
	}
}

void reset_game() {
	UBYTE i;

	game_running = 1u;
	health = MAX_HEALTH;
	score = 0;

	for (i = 0u; i < ENEMY_COUNT; i++) {
		reset_enemy(&enemies[i]);
	}

	for (i = 0u; i < 20u; i++) {		
		set_win_tiles(i, 0, 1u, 1u, blank_map);
	}

	update_score();
	update_health();

	reset_player();

	render_game_character(&player);
}

void main() {
	UBYTE is_render_frame = 0;
	UINT8 jpad = 0u, j, i;

	BGP_REG = 0x00; // Start palette all white for fade_in_white()

	init_sound();
	disable_interrupts();

	set_win_data(BACKGROUND_DATA_TILES, FONT_DATA_TILES, FontData);
	set_bkg_data(0u, BACKGROUND_DATA_TILES + 1, BackgroundData);
	set_bkg_data(FONT_DATA_TILES + BACKGROUND_DATA_TILES, 23u, TitleData);
	move_win(7u, 136u);

	for (j = 0u; j < 32u; j++) {
		for (i = 0u; i < 32u; i++) {		
			set_win_tiles(i, j, 1u, 1u, blank_map);
			set_bkg_tiles(i, j, 1u, 1u, blank_map);
		}
	}

	set_bkg_tiles(0, 0, 20u, 19u, TitleMap);
	set_win_tiles(0u, 0u, 11u, 1u, reset_prompt_map);
	move_win(43u, 108u);

	SHOW_BKG;
	SHOW_WIN;
	DISPLAY_ON;

	gbt_play(song_Data, 2, 7);
	gbt_loop(1);

	set_interrupts(VBL_IFLAG);

	enable_interrupts();
	fade_in_white();

	while (1) {
		wait_vbl_done();

		if (joypad() & J_START) {
			gbt_stop();
			wait_vbl_done();
			break;
		}
		
		gbt_update();
	}

	init_sound();
	fade_out_white();

	disable_interrupts();

	move_win(7u, 136u);
	set_bkg_tiles(0, 0, 32u, 32u, BackgroundMap);
	set_sprite_data(0u, 60u, SpriteData);
	set_up_player();

	for (i = 0u; i < BULLET_COUNT; i++) {
		set_up_bullet(&bullets[i]);
	}

	for (i = 0u; i < ENEMY_COUNT - SMALL_ENEMY_COUNT; i++) {
		set_up_enemy(&enemies[i], i % 2 == 0u);
	}

	for (i = ENEMY_COUNT - SMALL_ENEMY_COUNT; i < ENEMY_COUNT; i++) {
		set_up_small_enemy(&enemies[i], i % 2 == 0u);
	}

	reset_game();

	SHOW_SPRITES;

	enable_interrupts();

	fade_in_white();

	while (1) {
		wait_vbl_done();

		jpad = joypad();
		is_render_frame = !is_render_frame;

		if (is_render_frame) {
			update_player(jpad);

			if (game_running) {
				if (!player.is_destroying && (jpad & J_A)) {
					fire_bullet(player.direction);
				}
			} else if (jpad & J_START) {
				reset_game();
				continue;
			}

			for (i = 0u; i < BULLET_COUNT; i++) {
				if (!bullets[i].is_destroyed) {
					update_bullet(&bullets[i]);
				}

				render_game_character(&bullets[i]);
			}

			for (i = 0u; i < ENEMY_COUNT; i++) {
				if (enemies[i].is_destroyed) {
					continue;
				}

				update_enemy(&enemies[i]);
			}

			scroll_bkg(player.velocity_x, player.velocity_y);

			for (i = 0u; i < ENEMY_COUNT; i++) {
				if (enemies[i].is_destroyed) {
					reset_enemy(&enemies[i]);
				}

				render_game_character(&enemies[i]);
			}
		} else {
			for (i = 0u; i < ENEMY_COUNT; i++) {
				if (enemies[i].is_destroyed) {
					continue;
				}

				for (j = 0u; j < BULLET_COUNT; j++) {
					if (!bullets[j].is_destroyed) {
						if (did_collide(&bullets[j], &enemies[i])) {
							destroy_game_character(&bullets[j]);
							destroy_enemy(&enemies[i]);
							play_sound_explosion();
							score += 1u;
							update_score();
							break;
						}
					}
				}

				if (did_collide(&player, &enemies[i])) {
					health--;
					update_health();
					destroy_enemy(&enemies[i]);
					play_sound_explosion();

					if (health == 0u) {
						start_destroying_player();
						set_win_tiles(0u, 0u, 11u, 1u, reset_prompt_map);
					}
				}

				if (enemies[i].is_destroyed) {
					reset_enemy(&enemies[i]);
				}
			}
		}
	}
}
