#include <gb/gb.h>
#include "game-character.c"
#include "sprite-data.c"
#include "util.c"
#include "sound.c"

#define UPDATE_FRACTION 2u
#define MAX_VELOCITY 3
#define ACCELERATION 1
#define SPRITE_SIZE 8u

GameCharacter player;
GameCharacter bullet;
GameCharacter enemies[3u];

UBYTE update_count = 0u;
UBYTE next_sprite_index = 0u;

void move_game_character(GameCharacter* character, UINT8 x, UINT8 y) {
	move_sprite(character->sprite_ids[0u], x, y);
	move_sprite(character->sprite_ids[1u], x + SPRITE_SIZE, y);
	move_sprite(character->sprite_ids[2u], x, y + SPRITE_SIZE);
	move_sprite(character->sprite_ids[3u], x + SPRITE_SIZE, y + SPRITE_SIZE);
}

void update_game_character(GameCharacter* character) {
	character->x += character->velocity_x;
	character->y += character->velocity_y;

	// Slow sprite down
	// if (update_count == 0) {
	// 	if (character->velocity_x != 0) {
	// 		character->velocity_x += character->velocity_x > 0 ? -1 : 1;
	// 	}
	// 
	// 	if (character->velocity_y != 0) {
	// 		character->velocity_y += character->velocity_y > 0 ? -1 : 1;
	// 	}
	// }


	// Stop character at edges
	// if (character->y < 16) {
	// 	character->y = 16;
	// 	character->velocity_y = 0;
	// } else if (character->y > 144) {
	// 	character->y = 144;
	// 	character->velocity_y = 0;
	// }

	// if (character->x < 8) {
	// 	character->x = 8;
	// 	character->velocity_x = 0;
	// } else if (character->x > 152) {
	// 	character->x = 152;
	// 	character->velocity_x = 0;
	// }

	// Bounce character at edges
	// if (character->y < 16u) {
	// 	character->y = 16u;
	// 	character->velocity_y = -character->velocity_y;
	// } else if (character->y > 144u) {
	// 	character->y = 144u;
	// 	character->velocity_y = -character->velocity_y;
	// }

	// if (character->x < 8u) {
	// 	character->x = 8u;
	// 	character->velocity_x = -character->velocity_x;
	// } else if (character->x > 152u) {
	// 	character->x = 152u;
	// 	character->velocity_x = -character->velocity_x;
	// }

	// Wrap character at edges
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


	move_game_character(character, character->x, character->y);
}

UBYTE check_collision(GameCharacter* a, GameCharacter* b) {
	return (a->x >= b->x && a->x <= b->x + b->width && a->y >= b->y && a->y <= b->y + b->height) ||
		(b->x >= a->x && b->x <= a->x + a->width && b->y >= a->y && b->y <= a->y + a->height);
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
	player.width = 16;
	player.height = 16;

	set_sprite_tile(next_sprite_index, 0);
	player.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1);
	player.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2);
	player.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3);
	player.sprite_ids[3] = next_sprite_index++;

	move_game_character(&player, player.x, player.y);
}

void set_up_bullet() {
	bullet.x = 80;
	bullet.y = 130;
	bullet.velocity_x = 0;
	bullet.velocity_y = 0;
	bullet.width = 16;
	bullet.height = 16;

	set_sprite_tile(next_sprite_index, 20);
	bullet.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 21);
	bullet.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 22);
	bullet.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 23);
	bullet.sprite_ids[3] = next_sprite_index++;

	move_game_character(&bullet, bullet.x, bullet.y);
}

void set_up_enemy(GameCharacter* enemy, UINT8 x, UINT8 y, BYTE velocity_x, BYTE velocity_y) {
	enemy->x = x;
	enemy->y = y;
	enemy->velocity_x = velocity_x;
	enemy->velocity_y = velocity_y;
	enemy->width = 16;
	enemy->height = 16;

	set_sprite_tile(next_sprite_index, 16);
	enemy->sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 17);
	enemy->sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 18);
	enemy->sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 19);
	enemy->sprite_ids[3] = next_sprite_index++;

	move_game_character(enemy, enemy->x, enemy->y);
}

void fire_bullet(UINT8 jpad) {
	if (jpad == 0u) {
		jpad = player.direction;
	}

	if (jpad & J_LEFT) {
		bullet.velocity_x = -5;
	} else if (jpad & J_RIGHT) {
		bullet.velocity_x = 5;
	} else {
		bullet.velocity_x = 0;
	}

	if (jpad & J_UP) {
		bullet.velocity_y = -5;
	} else if (jpad & J_DOWN) {
		bullet.velocity_y = 5;
	} else {
		bullet.velocity_y = 0;
	}

	bullet.x = player.x + bullet.velocity_x;
	bullet.y = player.y + bullet.velocity_y;
	play_sound();
}

void main() {
	UINT8 jpad = 0u, j, i;

	set_sprite_data(0, 24, SpriteData);
	set_up_player();
	set_up_bullet();
	set_up_enemy(&enemies[0], 20, 50, 1, 1);
	set_up_enemy(&enemies[1], 100, 20, -1, 1);
	set_up_enemy(&enemies[2], 50, 100, -1, -1);

	DISPLAY_ON;
	SHOW_SPRITES;

	while (1) {
		update_count = (update_count + 1) % UPDATE_FRACTION;
		jpad = jpad | joypad(); // Keeps all input leading up to next update cycle

		if (update_count == 0u) {
			if (jpad & J_LEFT) {
				player.velocity_x -= ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 12);
				set_sprite_tile(player.sprite_ids[1], 13);
				set_sprite_tile(player.sprite_ids[2], 14);
				set_sprite_tile(player.sprite_ids[3], 15);
			}

			if (jpad & J_RIGHT) {
				player.velocity_x += ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 8);
				set_sprite_tile(player.sprite_ids[1], 9);
				set_sprite_tile(player.sprite_ids[2], 10);
				set_sprite_tile(player.sprite_ids[3], 11);
			}

			if (jpad & J_UP) {
				player.velocity_y -= ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 0);
				set_sprite_tile(player.sprite_ids[1], 1);
				set_sprite_tile(player.sprite_ids[2], 2);
				set_sprite_tile(player.sprite_ids[3], 3);
			}

			if (jpad & J_DOWN) {
				player.velocity_y += ACCELERATION;
				player.direction = jpad;

				set_sprite_tile(player.sprite_ids[0], 4);
				set_sprite_tile(player.sprite_ids[1], 5);
				set_sprite_tile(player.sprite_ids[2], 6);
				set_sprite_tile(player.sprite_ids[3], 7);
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

			update_game_character(&player);
			update_game_character(&bullet);

			for (i = 0; i < 3; i++) {
				update_game_character(&enemies[i]);

				if (check_collision(&player, &enemies[i])) {
					collide_game_characters(&player, &enemies[i]);
				}

				for (j = 0; j < 3; j++) {
					if (i == j) {
						continue;
					}

					if (check_collision(&enemies[i], &enemies[j])) {

						collide_game_characters(&enemies[j], &enemies[i]);
						// enemies[i].velocity_y = -enemies[i].velocity_y;
						// enemies[i].velocity_x = -enemies[i].velocity_x;
						// enemies[j].velocity_y = -enemies[j].velocity_y;
						// enemies[j].velocity_x = -enemies[j].velocity_x;
					}
				}
			}
		}

			
		wait_vbl_done();
	}
}
