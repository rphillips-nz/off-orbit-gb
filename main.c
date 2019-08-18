#include <gb/gb.h>
#include <rand.h>
#include "game-character.c"
#include "sprite-data.c"
#include "sound.c"
#include "font.c"
#include "window-map.c"

#define UPDATE_FRACTION 2u
#define MAX_VELOCITY 2
#define ACCELERATION 1
#define SPRITE_SIZE 8u

GameCharacter player;
GameCharacter bullet;
GameCharacter enemies[3u];

UBYTE update_count = 0u;
UBYTE next_sprite_index = 0u;
UBYTE score = 0u;

const unsigned char blank_map[1] = {0x00};

INT16 rand_range(INT16 min_n, INT16 max_n) {
	return rand() % (max_n - min_n + 1) + min_n;
}

void destroy_game_character(GameCharacter* character) {
	character->is_destroyed = 1;
	character->x = -16;
	character->y = -16;
	character->velocity_x = 0;
	character->velocity_y = 0;
}

void move_game_character(GameCharacter* character, UINT8 x, UINT8 y) {
	move_sprite(character->sprite_ids[0u], x, y);
	move_sprite(character->sprite_ids[1u], x + SPRITE_SIZE, y);
	move_sprite(character->sprite_ids[2u], x, y + SPRITE_SIZE);
	move_sprite(character->sprite_ids[3u], x + SPRITE_SIZE, y + SPRITE_SIZE);
}

void update_game_character(GameCharacter* character, UBYTE allow_wrap) {
	character->x += character->velocity_x;
	character->y += character->velocity_y;

	if (allow_wrap) {
		if (character->y > 200) {
			character->y = 160;
		} else if (character->y > 160) {
			character->y = 0;
		}

		if (character->x > 200) {
			character->x = 160;
		} else if (character->x > 168) {
			character->x = 0;
		}
	} else {
		if (character->y > 200 || character->y > 160 || character->x > 200 || character->x > 168) {
			destroy_game_character(character);
		}
	}

	move_game_character(character, character->x, character->y);
}

UBYTE check_collision(GameCharacter* a, GameCharacter* b) {
	return !a->is_destroyed && !b->is_destroyed && (
		(a->x >= b->x && a->x <= b->x + b->width && a->y >= b->y && a->y <= b->y + b->height) ||
		(b->x >= a->x && b->x <= a->x + a->width && b->y >= a->y && b->y <= a->y + a->height)
	);
}

void collide_game_characters(GameCharacter* a, GameCharacter* b) {
	INT8 delta_x, delta_y, move_x, move_y;

	if (a->x < b->x) {
		delta_x = b->x - a->x;
		move_x = (a->width - delta_x) / 2;
		a->x = a->x - move_x - 1;
		b->x = b->x + move_x + 1;
		delta_x = b->x - a->x;
	} else {
		delta_x = a->x - b->x;
		move_x = (b->width - delta_x) / 2;
		a->x = a->x - move_x - 1;
		b->x = b->x + move_x + 1;
		delta_x = a->x - b->x;
	}

	if (a->y < b->y) {
		delta_y = b->y - a->y;
		move_y = (a->height - delta_y) / 2;
		a->y = a->y - move_y - 1;
		b->y = b->y + move_y + 1;
		delta_y = b->y - a->y;
	} else {
		delta_y = a->y - b->y;
		move_y = (b->height - delta_y) / 2;
		a->y = a->y - move_y - 1;
		b->y = b->y + move_y + 1;
		delta_y = a->y - b->y;
	}

	if (delta_y < delta_x) {
		b->velocity_x = -b->velocity_x;
		a->velocity_x = -a->velocity_x;
	} else {
		b->velocity_y = -b->velocity_y;
		a->velocity_y = -a->velocity_y;
	}
}

void set_up_player() {
	player.direction = J_UP;
	player.x = 80;
	player.y = 130;
	player.velocity_x = 0;
	player.velocity_y = 0;
	player.width = 16u;
	player.height = 16u;

	set_sprite_tile(next_sprite_index, 0u);
	player.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1u);
	player.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2u);
	player.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3u);
	player.sprite_ids[3] = next_sprite_index++;
}

void set_up_bullet() {
	bullet.x = -16;
	bullet.y = -16;
	bullet.velocity_x = 0;
	bullet.velocity_y = 0;
	bullet.width = 16u;
	bullet.height = 16u;

	set_sprite_tile(next_sprite_index, 20u);
	bullet.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 21u);
	bullet.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 22u);
	bullet.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 23u);
	bullet.sprite_ids[3] = next_sprite_index++;
}

void set_up_enemy(GameCharacter* enemy, UINT8 x, UINT8 y, BYTE velocity_x, BYTE velocity_y) {
	enemy->x = x;
	enemy->y = y;
	enemy->velocity_x = velocity_x;
	enemy->velocity_y = velocity_y;
	enemy->width = 16u;
	enemy->height = 16u;

	set_sprite_tile(next_sprite_index, 16u);
	enemy->sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 17u);
	enemy->sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 18u);
	enemy->sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 19u);
	enemy->sprite_ids[3] = next_sprite_index++;
}

void fire_bullet(UINT8 direction) {
	if (!bullet.is_destroyed) {
		return;
	}

	if (direction & J_LEFT) {
		bullet.velocity_x = -5;
	} else if (direction & J_RIGHT) {
		bullet.velocity_x = 5;
	} else {
		bullet.velocity_x = 0;
	}

	if (direction & J_UP) {
		bullet.velocity_y = -5;
	} else if (direction & J_DOWN) {
		bullet.velocity_y = 5;
	} else {
		bullet.velocity_y = 0;
	}

	bullet.x = player.x + bullet.velocity_x;
	bullet.y = player.y + bullet.velocity_y;
	bullet.is_destroyed = 0;
}

void update_score() {
	UINT8 digit_map[1u];
	UBYTE score_iterator = score;
	INT8 digit_render_count = 0u;

	// if (time > sessionhighscore) {
	// 	sessionhighscore = time;
	// }

	do {
		digit_map[0u] = score_iterator % 10u + 1u; // + 1 is the win data offset where the digits start
		set_win_tiles(19u - digit_render_count, 0u, 1u, 1u, digit_map);
		digit_render_count++;
		score_iterator /= 10u;
	} while (score_iterator > 0u);
}

void main() {
	UINT8 jpad = 0u, j, i;

	disable_interrupts();

	set_sprite_data(0u, 24u, SpriteData);
	set_up_player();
	set_up_bullet();

	initrand(DIV_REG);
	set_up_enemy(&enemies[0], rand_range(0, 101), rand_range(0, 101), 1, 1);
	set_up_enemy(&enemies[1], rand_range(0, 101), rand_range(0, 101), -1, 1);
	set_up_enemy(&enemies[2], rand_range(0, 101), rand_range(0, 101), -1, -1);

	set_win_data(0u, 36u, FontData);
	move_win(7u, 136u);

	for (j = 0u; j != 32u; j++) {
		for (i = 0u; i != 32u; i++) {		
			set_win_tiles(i, j, 1u, 1u, blank_map);
			set_bkg_tiles(i, j, 1u, 1u, blank_map);
		}
	}

	update_score();

	SHOW_SPRITES;
	SHOW_BKG;
	SHOW_WIN;
	DISPLAY_ON;

	enable_interrupts();

	while (1) {
		wait_vbl_done();

		update_count = (update_count + 1u) % UPDATE_FRACTION;
		jpad = jpad | joypad(); // Keeps all input leading up to next update cycle

		if (update_count == 0u) {
			if (jpad & J_LEFT) {
				player.velocity_x -= ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 12u);
				set_sprite_tile(player.sprite_ids[1], 13u);
				set_sprite_tile(player.sprite_ids[2], 14u);
				set_sprite_tile(player.sprite_ids[3], 15u);
			}

			if (jpad & J_RIGHT) {
				player.velocity_x += ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 8u);
				set_sprite_tile(player.sprite_ids[1], 9u);
				set_sprite_tile(player.sprite_ids[2], 10u);
				set_sprite_tile(player.sprite_ids[3], 11u);
			}

			if (jpad & J_UP) {
				player.velocity_y -= ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 0u);
				set_sprite_tile(player.sprite_ids[1], 1u);
				set_sprite_tile(player.sprite_ids[2], 2u);
				set_sprite_tile(player.sprite_ids[3], 3u);
			}

			if (jpad & J_DOWN) {
				player.velocity_y += ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 4u);
				set_sprite_tile(player.sprite_ids[1], 5u);
				set_sprite_tile(player.sprite_ids[2], 6u);
				set_sprite_tile(player.sprite_ids[3], 7u);
			}

			if (jpad & J_A) {
				fire_bullet(player.direction);
			}

			jpad = 0u;

			if (player.velocity_x < -MAX_VELOCITY) {
				player.velocity_x = -MAX_VELOCITY;
			} else if (player.velocity_x > MAX_VELOCITY) {
				player.velocity_x = MAX_VELOCITY;
			}

			if (player.velocity_y < -MAX_VELOCITY) {
				player.velocity_y = -MAX_VELOCITY;
			} else if (player.velocity_y > MAX_VELOCITY) {
				player.velocity_y = MAX_VELOCITY;
			}

			for (i = 0u; i < 3u; i++) {
				if (check_collision(&bullet, &enemies[i])) {
					destroy_game_character(&bullet);
					destroy_game_character(&enemies[i]);
					score += 1u;
					update_score();
					continue;
				}

				if (check_collision(&player, &enemies[i])) {
					collide_game_characters(&player, &enemies[i]);
				}

				for (j = 0u; j < 3u; j++) {
					if (i == j) {
						continue;
					}

					if (check_collision(&enemies[i], &enemies[j])) {
						collide_game_characters(&enemies[j], &enemies[i]);
					}
				}

				update_game_character(&enemies[i], 1u);
			}

			update_game_character(&player, 1u);
			update_game_character(&bullet, 0u);
		}			
	}
}
