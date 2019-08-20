#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>
#include "game-character.c"
#include "sprite-data.c"
#include "sound.c"
#include "font.c"
#include "background-data.c"
#include "background-map.c"

#define MAX_VELOCITY 4
#define ACCELERATION 1
#define SPRITE_SIZE 8u

#define FIRE_RATE 15u
#define BULLET_COUNT 2u
#define ENEMY_COUNT 4u

GameCharacter player;
GameCharacter bullets[BULLET_COUNT];
GameCharacter enemies[ENEMY_COUNT];

UBYTE next_sprite_index = 0u;
UBYTE score = 0u;
UINT16 time_last_fired;

const unsigned char blank_map[1] = {0x00};

INT8 rand_range(INT8 min_n, INT8 max_n) {
	max_n++;
	return min_n + abs(rand()) % (max_n - min_n);
}

void reset_enemy(GameCharacter* enemy) {
	INT8 x, y;

	initrand(DIV_REG);

	x = rand_range(8, 168);
	y = rand_range(16, 160);

	if (x < 88) {
		enemy->x = x - 50;
		enemy->velocity_x = rand_range(1, 2);
	} else {
		enemy->x = x + 50;
		enemy->velocity_x = -rand_range(1, 2);
	}

	if (y < 88) {
		enemy->y = y - 50;
		enemy->velocity_y = rand_range(1, 2);
	} else {
		enemy->y = y + 50;
		enemy->velocity_y = -rand_range(1, 2);
	}

	enemy->width = 16u;
	enemy->height = 16u;
	enemy->is_destroyed = 0;
}

void destroy_game_character(GameCharacter* character) {
	character->is_destroyed = 1;
	character->x = -16;
	character->y = -16;
	character->velocity_x = 0;
	character->velocity_y = 0;
}

void render_game_character(GameCharacter* character) {
	move_sprite(character->sprite_ids[0u], character->x, character->y);
	move_sprite(character->sprite_ids[1u], character->x + SPRITE_SIZE, character->y);
	move_sprite(character->sprite_ids[2u], character->x, character->y + SPRITE_SIZE);
	move_sprite(character->sprite_ids[3u], character->x + SPRITE_SIZE, character->y + SPRITE_SIZE);
}

void update_game_character(GameCharacter* character) {
	character->x -= player.velocity_x;
	character->y -= player.velocity_y;
	character->x += character->velocity_x;
	character->y += character->velocity_y;
}

void update_bullet(GameCharacter* bullet) {
	UINT8 distance_x, distance_y;

	update_game_character(bullet);

	distance_x = player.x > bullet->x ? player.x - bullet->x : bullet->x - player.x;
	if (distance_x > 80) {
		destroy_game_character(bullet);
		return;
	}

	distance_y = player.y > bullet->y ? player.y - bullet->y : bullet->y - player.y;
	if (distance_y > 72) {
		destroy_game_character(bullet);
	}
}

void update_enemy(GameCharacter* enemy) {
	UINT8 distance_x, distance_y;

	// if (sys_time % 60 == 0) {
	// 	enemy->velocity_y += player.y < enemy->y ? -1 : 1;
	// 	enemy->velocity_x += player.x < enemy->x ? -1 : 1;
	// }

	update_game_character(enemy);

	distance_x = player.x > enemy->x ? player.x - enemy->x : enemy->x - player.x;
	if (distance_x > 200) {
		reset_enemy(enemy);
		return;
	}

	distance_y = player.y > enemy->y ? player.y - enemy->y : enemy->y - player.y;
	if (distance_y > 200) {
		reset_enemy(enemy);
	}
}

UBYTE check_collision(GameCharacter* a, GameCharacter* b) {
	INT8 a_left, a_top, a_right, a_bottom, b_left, b_top, b_right, b_bottom;

	if (a->is_destroyed || b->is_destroyed) {
		return 0;
	}

	a_left = a->x;
	a_top = a->y;
	a_right = a->x + a->width;
	a_bottom = a->y + a->height;
	b_left = b->x;
	b_top = b->y;
	b_right = b->x + b->width;
	b_bottom = b->y + b->height;

	return !(a_bottom < b_top || a_top > b_bottom || a_right < b_left || a_left > b_right);
}

void resolve_collision(GameCharacter* a, GameCharacter* b) {
	INT8 a_mid_x = a->x + (a->width / 2);
	INT8 a_mid_y = a->y + (a->height / 2);
	INT8 b_mid_x = b->x + (b->width / 2);
	INT8 b_mid_y = b->y + (b->height / 2);
	INT8 dx = (b_mid_x - a_mid_x) / (b->width / 2);
	INT8 dy = (b_mid_y - a_mid_y) / (b->height / 2);
	INT8 absDX = abs(dx);
	INT8 absDY = abs(dy);

	if (absDX > absDY) { // collision is on the sides
		if (dx < 0) { // a is moving left
			a->x = b->x + b->width;
		} else { // a is moving right
			a->x = b->x - a->width;
		}

		a->velocity_x = -a->velocity_x;
	} else { // collision is on the top or bottom
		if (dy < 0) { // a is moving down
			a->y = b->y + b->height;
		} else { // a is moving up
			a->y = b->y - a->height;
		}

		a->velocity_y = -a->velocity_y;
	}
}

void set_up_player() {
	player.direction = J_UP;
	player.x = 80;
	player.y = 80;
	player.velocity_x = 0;
	player.velocity_y = 0;
	player.width = 16u;
	player.height = 16u;
	player.is_destroyed = 0;

	set_sprite_tile(next_sprite_index, 0u);
	player.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1u);
	player.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2u);
	player.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3u);
	player.sprite_ids[3] = next_sprite_index++;

	render_game_character(&player);
}

void set_up_bullet(GameCharacter* bullet) {
	bullet->x = -16;
	bullet->y = -16;
	bullet->velocity_x = 0;
	bullet->velocity_y = 0;
	bullet->width = 16u;
	bullet->height = 16u;
	bullet->is_destroyed = 1;

	set_sprite_tile(next_sprite_index, 20u);
	bullet->sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 21u);
	bullet->sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 22u);
	bullet->sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 23u);
	bullet->sprite_ids[3] = next_sprite_index++;
}

void set_up_enemy(GameCharacter* enemy) {
	reset_enemy(enemy);

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
	GameCharacter* bullet;
	UBYTE i;

	for (i = 0u; i < BULLET_COUNT; i++) {
		if (bullets[i].is_destroyed) {
			bullet = &bullets[i];
			break;
		}
	}

	if (!bullet) {
		return;
	}

	if (sys_time - time_last_fired < FIRE_RATE) {
		return;
	}

	time_last_fired = sys_time;

	if (direction & J_LEFT) {
		player.velocity_x += ACCELERATION;
		if (player.velocity_x > MAX_VELOCITY) {
			player.velocity_x = MAX_VELOCITY;
		}

		if (player.velocity_x < 0) {
			bullet->velocity_x = player.velocity_x - 5;
		} else {
			bullet->velocity_x = -5;
		}
	} else if (direction & J_RIGHT) {
		player.velocity_x -= ACCELERATION;
		if (player.velocity_x < -MAX_VELOCITY) {
			player.velocity_x = -MAX_VELOCITY;
		}

		if (player.velocity_x > 0) {
			bullet->velocity_x = player.velocity_x + 5;
		} else {
			bullet->velocity_x = 5;
		}
	} else {
		bullet->velocity_x = 0;
	}

	if (direction & J_UP) {
		player.velocity_y += ACCELERATION;
		if (player.velocity_y > MAX_VELOCITY) {
			player.velocity_y = MAX_VELOCITY;
		}
		
		if (player.velocity_y < 0) {
			bullet->velocity_y = player.velocity_y - 5;
		} else {
			bullet->velocity_y = -5;
		}
	} else if (direction & J_DOWN) {
		player.velocity_y -= ACCELERATION;
		if (player.velocity_y < -MAX_VELOCITY) {
			player.velocity_y = -MAX_VELOCITY;
		}
		
		if (player.velocity_y > 0) {
			bullet->velocity_y = player.velocity_y + 5;
		} else {
			bullet->velocity_y = 5;
		}
	} else {
		bullet->velocity_y = 0;
	}

	bullet->x = player.x + bullet->velocity_x;
	bullet->y = player.y + bullet->velocity_y;
	bullet->is_destroyed = 0;
}

void update_score() {
	UINT8 digit_map[1u];
	UBYTE score_iterator = score;
	INT8 digit_render_count = 0u;

	// if (time > sessionhighscore) {
	// 	sessionhighscore = time;
	// }

	do {
		digit_map[0u] = score_iterator % 10u + 6u; // + 6 is the win data offset where the digits start
		set_win_tiles(19u - digit_render_count, 0u, 1u, 1u, digit_map);
		digit_render_count++;
		score_iterator /= 10u;
	} while (score_iterator > 0u);
}

void turn_player(UINT8 jpad) {
	if (jpad & J_LEFT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 12u);
		set_sprite_tile(player.sprite_ids[1], 13u);
		set_sprite_tile(player.sprite_ids[2], 14u);
		set_sprite_tile(player.sprite_ids[3], 15u);
	}

	if (jpad & J_RIGHT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 8u);
		set_sprite_tile(player.sprite_ids[1], 9u);
		set_sprite_tile(player.sprite_ids[2], 10u);
		set_sprite_tile(player.sprite_ids[3], 11u);
	}

	if (jpad & J_UP) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 0u);
		set_sprite_tile(player.sprite_ids[1], 1u);
		set_sprite_tile(player.sprite_ids[2], 2u);
		set_sprite_tile(player.sprite_ids[3], 3u);
	}

	if (jpad & J_DOWN) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 4u);
		set_sprite_tile(player.sprite_ids[1], 5u);
		set_sprite_tile(player.sprite_ids[2], 6u);
		set_sprite_tile(player.sprite_ids[3], 7u);
	}
}

void main() {
	UINT8 jpad = 0u, j, i;

	disable_interrupts();

	set_win_data(5u, 36u, FontData);
	set_bkg_data(0u, 6u, BackgroundData);
	move_win(7u, 136u);

	for (j = 0u; j < 32u; j++) {
		for (i = 0u; i < 32u; i++) {		
			set_win_tiles(i, j, 1u, 1u, blank_map);
			set_bkg_tiles(i, j, 1u, 1u, blank_map);
		}
	}

	set_bkg_tiles(0, 0, 40, 34, BackgroundMap);
	update_score();

	set_sprite_data(0u, 24u, SpriteData);
	set_up_player();

	for (i = 0u; i < BULLET_COUNT; i++) {
		set_up_bullet(&bullets[i]);
	}

	for (i = 0u; i < ENEMY_COUNT; i++) {
		set_up_enemy(&enemies[i]);
	}

	SHOW_SPRITES;
	SHOW_BKG;
	SHOW_WIN;
	DISPLAY_ON;

	enable_interrupts();

	while (1) {
		wait_vbl_done();

		jpad = joypad();

		turn_player(jpad);

		if (jpad & J_A) {
			fire_bullet(player.direction);
		}

		for (i = 0u; i < BULLET_COUNT; i++) {
			if (bullets[i].is_destroyed) {
				continue;
			}

			update_bullet(&bullets[i], 0u);

			for (j = 0u; j < ENEMY_COUNT; j++) {
				if (check_collision(&bullets[i], &enemies[j])) {
					destroy_game_character(&bullets[i]);
					destroy_game_character(&enemies[j]);
					score += 1u;
					update_score();
					break;
				}
			}
		}

		for (i = 0u; i < ENEMY_COUNT; i++) {
			if (enemies[i].is_destroyed) {
				continue;
			}

			update_enemy(&enemies[i]);

			for (j = 0u; j < ENEMY_COUNT; j++) {
				if (i != j && check_collision(&enemies[i], &enemies[j])) {
					resolve_collision(&enemies[i], &enemies[j]);
				}
			}

			if (check_collision(&player, &enemies[i])) {
				resolve_collision(&enemies[i], &player);
			}
		}

		scroll_bkg(player.velocity_x, player.velocity_y);

		for (i = 0u; i < ENEMY_COUNT; i++) {
			if (enemies[i].is_destroyed) {
				reset_enemy(&enemies[i]);
			}

			render_game_character(&enemies[i]);
		}

		for (i = 0u; i < BULLET_COUNT; i++) {
			render_game_character(&bullets[i]);
		}
	}
}
