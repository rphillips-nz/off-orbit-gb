#ifndef _OO_ENEMY_H_
#define _OO_ENEMY_H_

#include <gb/gb.h>
#include "game-character.h"

void animate_enemy(GameCharacter* enemy);

void set_up_enemy(GameCharacter* enemy, UBYTE is_alternate);

void set_up_small_enemy(GameCharacter* enemy, UBYTE is_alternate);

void update_enemy(GameCharacter* enemy);

void reset_enemy(GameCharacter* enemy);

void destroy_enemy(GameCharacter* enemy);

#endif
